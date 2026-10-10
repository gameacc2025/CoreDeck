#include "include/database/Migration.h"
#include "include/global/Logger.hpp"
#include "include/global/Configs.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <3rdparty/SQLiteCpp/include/SQLiteCpp.h>
#include <algorithm>
#include <set>
#include <memory>

namespace Configs {

static constexpr const char* MIGRATION_VERSION_KEY = "coredeck_migration_version";
static constexpr const char* CURRENT_MIGRATION_VERSION = "1";

bool isMigrationNeeded(Database& db) {
    std::string version = getMigrationVersion(db);
    if (version == CURRENT_MIGRATION_VERSION) {
        return false;
    }
    auto q = db.query("SELECT 1 FROM sqlite_master WHERE type='table' AND name='instances'");
    return !q || !q->executeStep();
}

std::string getMigrationVersion(Database& db) {
    try {
        auto q = db.query("SELECT value FROM settings WHERE key = ?", MIGRATION_VERSION_KEY);
        if (q && q->executeStep()) {
            return q->getColumn(0).getText();
        }
    } catch (...) {}
    return "0";
}

void setMigrationVersion(Database& db, const std::string& version) {
    db.exec("INSERT OR REPLACE INTO settings (key, value) VALUES (?, ?)", MIGRATION_VERSION_KEY, version);
}

bool tableExists(Database& db, const std::string& table) {
    try {
        auto q = db.query("SELECT 1 FROM sqlite_master WHERE type='table' AND name=?", table);
        return q && q->executeStep();
    } catch (...) {
        return false;
    }
}

bool columnExists(Database& db, const std::string& table, const std::string& column) {
    if (!tableExists(db, table)) return false;
    try {
        auto q = db.query("PRAGMA table_info(" + table + ")");
        while (q && q->executeStep()) {
            if (q->getColumn(1).getText() == column) return true;
        }
    } catch (...) {}
    return false;
}

void createInstancesTable(Database& db) {
    db.exec(R"(
        CREATE TABLE IF NOT EXISTS instances (
            id TEXT PRIMARY KEY,
            name TEXT NOT NULL DEFAULT '',
            core_type TEXT NOT NULL DEFAULT 'xray',
            socks_port INTEGER NOT NULL DEFAULT 10808,
            http_port INTEGER NOT NULL DEFAULT 10809,
            api_port INTEGER NOT NULL DEFAULT 10810,
            bind_address TEXT NOT NULL DEFAULT '127.0.0.1',
            auto_start INTEGER NOT NULL DEFAULT 0,
            restart_on_crash INTEGER NOT NULL DEFAULT 0,
            restore_last_profile INTEGER NOT NULL DEFAULT 1,
            startup_delay_ms INTEGER NOT NULL DEFAULT 0,
            last_profile_id INTEGER NOT NULL DEFAULT -1,
            last_active_profile_id INTEGER NOT NULL DEFAULT -1,
            dns_settings_json TEXT NOT NULL DEFAULT '{}',
            core_config_json TEXT NOT NULL DEFAULT '{}',
            created_at INTEGER NOT NULL DEFAULT (strftime('%s', 'now')),
            updated_at INTEGER NOT NULL DEFAULT (strftime('%s', 'now'))
        )
    )");
}

void createInstanceSettingsTable(Database& db) {
    db.exec(R"(
        CREATE TABLE IF NOT EXISTS instance_settings (
            instance_id TEXT NOT NULL,
            key TEXT NOT NULL,
            value TEXT NOT NULL,
            PRIMARY KEY (instance_id, key)
        )
    )");
    db.exec("CREATE INDEX IF NOT EXISTS idx_instance_settings_instance_id ON instance_settings(instance_id)");
}

void createPortAllocationsTable(Database& db) {
    db.exec(R"(
        CREATE TABLE IF NOT EXISTS port_allocations (
            instance_id TEXT NOT NULL,
            port_type TEXT NOT NULL,
            port INTEGER NOT NULL,
            PRIMARY KEY (instance_id, port_type)
        )
    )");
    db.exec("CREATE UNIQUE INDEX IF NOT EXISTS idx_port_allocations_port ON port_allocations(port)");
}

std::string generateDefaultInstanceUuid() {
    return QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
}

qint64 currentTimestamp() {
    return QDateTime::currentSecsSinceEpoch();
}

bool backupDatabaseFile(const std::string& dbPath) {
    const QString srcPath = QString::fromStdString(dbPath);
    const QString backupPath = srcPath + ".pre-migration-backup-" + QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss");
    
    if (!QFile::copy(srcPath, backupPath)) {
        LOG_ERROR(QString("Failed to create database backup at %1").arg(backupPath));
        return false;
    }
    LOG_INFO(QString("Database backup created at %1").arg(backupPath));
    return true;
}

MigrationResult runThroneToCoreDeckMigration(Database& db, const std::string& dbPath) {
    MigrationResult result;
    
    if (!isMigrationNeeded(db)) {
        result.migrated = false;
        result.defaultInstanceId = "";
        return result;
    }

    LOG_INFO("Starting Throne -> CoreDeck database migration...");

    if (!backupDatabaseFile(dbPath)) {
        result.error = "Failed to create database backup before migration";
        return result;
    }

    if (!db.transaction("ThroneToCoreDeckMigration", [&]() {
        try {
            std::string defaultInstanceId = generateDefaultInstanceUuid();
            result.defaultInstanceId = defaultInstanceId;
            qint64 now = currentTimestamp();

            createInstancesTable(db);
            createInstanceSettingsTable(db);
            createPortAllocationsTable(db);

            db.exec(R"(
                INSERT INTO instances (
                    id, name, core_type, socks_port, http_port, api_port,
                    bind_address, auto_start, restart_on_crash, restore_last_profile,
                    startup_delay_ms, last_profile_id, last_active_profile_id,
                    dns_settings_json, core_config_json, created_at, updated_at
                ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
            )",
                defaultInstanceId,
                "Default",
                "xray",
                10808, 10809, 10810,
                "127.0.0.1",
                0, 0, 1,
                0, -1, -1,
                "{}", "{}",
                now, now
            );

            if (!columnExists(db, "groups", "instance_id")) {
                db.exec("ALTER TABLE groups ADD COLUMN instance_id TEXT");
            }

            if (!columnExists(db, "groups", "instance_id")) {
                result.error = "Failed to add instance_id column to groups table";
                return false;
            }

            db.exec("UPDATE groups SET instance_id = ? WHERE instance_id IS NULL", defaultInstanceId);

            auto groupsQuery = db.query("SELECT COUNT(*) FROM groups");
            int totalGroups = 0;
            if (groupsQuery && groupsQuery->executeStep()) {
                totalGroups = groupsQuery->getColumn(0).getInt();
            }
            result.groupsMigrated = totalGroups;

            db.exec(R"(
                INSERT INTO port_allocations (instance_id, port_type, port) VALUES
                (?, 'socks', 10808),
                (?, 'http', 10809),
                (?, 'api', 10810)
            )", defaultInstanceId, defaultInstanceId, defaultInstanceId);

            auto settingsQuery = db.query("SELECT key, value FROM settings");
            int settingsCount = 0;
            std::vector<std::pair<std::string, std::string>> instanceSettings;
            
            if (settingsQuery) {
                while (settingsQuery->executeStep()) {
                    std::string key = settingsQuery->getColumn(0).getText();
                    std::string value = settingsQuery->getColumn(1).getText();
                    
                    static const std::set<std::string> globalOnlyKeys = {
                        "language", "theme", "tray_icon", "start_minimized",
                        "check_updates", "auto_update", "log_level", "log_retention_days",
                        "shortcuts", "xray_vless_preference", "direct_dns_disable_ipv6",
                        "remote_dns_disable_ipv6", "route_auto_update_last", "dns_predefined_rules",
                        "extra_core_paths", "log_include_keyword", "log_include_regex",
                        "log_exclude_keyword", "log_exclude_regex", "warp_ifc_addrs",
                        "vpn_private_ranges", "xray_geoip_url_history", "xray_geosite_url_history",
                        "dial_bind_ifc_history", "dial_inet4_bind_history", "dial_inet6_bind_history",
                        "warp_reserved", "warp_masque_ifc_addrs", "warp_api_hosts"
                    };
                    
                    static const std::set<std::string> instanceKeys = {
                        "socks_port", "http_port", "api_port", "bind_address",
                        "auto_start", "restart_on_crash", "restore_last_profile",
                        "startup_delay_ms", "last_profile_id", "last_active_profile_id"
                    };
                    
                    if (globalOnlyKeys.count(key) == 0 && instanceKeys.count(key) == 0) {
                        instanceSettings.emplace_back(key, value);
                    }
                    
                    if (instanceKeys.count(key) > 0) {
                        instanceSettings.emplace_back(key, value);
                        settingsCount++;
                    }
                }
            }
            
            for (const auto& kv : instanceSettings) {
                db.exec("INSERT OR REPLACE INTO instance_settings (instance_id, key, value) VALUES (?, ?, ?)",
                    defaultInstanceId, kv.first, kv.second);
            }
            result.settingsMigrated = settingsCount;

            db.exec("CREATE INDEX IF NOT EXISTS idx_groups_instance_id ON groups(instance_id)");
            
            db.exec("PRAGMA foreign_keys = ON");
            try {
                db.exec("ALTER TABLE groups ADD CONSTRAINT fk_groups_instance_id FOREIGN KEY (instance_id) REFERENCES instances(id) ON DELETE CASCADE");
            } catch (...) {
                LOG_WARN("Could not add foreign key constraint on groups.instance_id (may already exist or SQLite version limitation)");
            }

            setMigrationVersion(db, CURRENT_MIGRATION_VERSION);
            
            LOG_INFO(QString("Migration complete: Default Instance %1, %2 groups, %3 settings migrated")
                .arg(QString::fromStdString(defaultInstanceId))
                .arg(totalGroups)
                .arg(settingsCount));
            
            return true;
        } catch (const std::exception& e) {
            result.error = e.what();
            LOG_ERROR(QString("Migration failed: %1").arg(e.what()));
            return false;
        }
    })) {
        return result;
    }
    
    result.migrated = true;
    return result;
}

} // namespace Configs