#include "include/database/InstanceRepository.h"
#include "include/global/Logger.hpp"
#include "include/global/Utils.hpp"
#include <QUuid>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <algorithm>

namespace Configs {

InstanceRepository::InstanceRepository(Database& database) : db(database) {
    createTables();
}

void InstanceRepository::createTables() {
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
    
    db.exec(R"(
        CREATE TABLE IF NOT EXISTS instance_settings (
            instance_id TEXT NOT NULL,
            key TEXT NOT NULL,
            value TEXT NOT NULL,
            PRIMARY KEY (instance_id, key)
        )
    )");
    db.exec("CREATE INDEX IF NOT EXISTS idx_instance_settings_instance_id ON instance_settings(instance_id)");
    
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

std::shared_ptr<Instance> InstanceRepository::instanceFromRow(SQLite::Statement& query) const {
    auto instance = std::make_shared<Instance>();
    
    instance->id = QUuid::fromString(QString::fromStdString(query.getColumn(0).getText()));
    instance->name = QString::fromStdString(query.getColumn(1).getText());
    
    QString coreTypeStr = QString::fromStdString(query.getColumn(2).getText());
    if (coreTypeStr.compare("xray", Qt::CaseInsensitive) == 0) {
        instance->core_type = CoreType::Xray;
    } else if (coreTypeStr.compare("singbox", Qt::CaseInsensitive) == 0 || 
               coreTypeStr.compare("sing-box", Qt::CaseInsensitive) == 0) {
        instance->core_type = CoreType::SingBox;
    } else if (coreTypeStr.compare("external", Qt::CaseInsensitive) == 0) {
        instance->core_type = CoreType::External;
    } else {
        instance->core_type = CoreType::Xray;
    }
    
    instance->socks_port = query.getColumn(3).getInt();
    instance->http_port = query.getColumn(4).getInt();
    instance->api_port = query.getColumn(5).getInt();
    instance->bind_address = QString::fromStdString(query.getColumn(6).getText());
    instance->auto_start = query.getColumn(7).getInt() != 0;
    instance->restart_on_crash = query.getColumn(8).getInt() != 0;
    instance->restore_last_profile = query.getColumn(9).getInt() != 0;
    instance->startup_delay_ms = query.getColumn(10).getInt();
    instance->last_profile_id = query.getColumn(11).getInt();
    instance->last_active_profile_id = query.getColumn(12).getInt();
    
    QString dnsJson = QString::fromStdString(query.getColumn(13).getText());
    QString coreJson = QString::fromStdString(query.getColumn(14).getText());
    instance->created_at = query.getColumn(15).getInt64();
    instance->updated_at = query.getColumn(16).getInt64();
    
    if (!dnsJson.isEmpty() && dnsJson != "{}") {
        QJsonDocument dnsDoc = QJsonDocument::fromJson(dnsJson.toUtf8());
        if (dnsDoc.isObject()) {
            instance->dns_settings = InstanceDNSSettings::FromJson(dnsDoc.object());
        }
    }
    
    if (!coreJson.isEmpty() && coreJson != "{}") {
        QJsonDocument coreDoc = QJsonDocument::fromJson(coreJson.toUtf8());
        if (coreDoc.isObject()) {
            instance->core_config = InstanceCoreConfig::FromJson(coreDoc.object());
        }
    }
    
    return instance;
}

void InstanceRepository::bindInstance(SQLite::Statement& query, const Instance& instance, int startIdx) const {
    query.bind(startIdx, instance.id.toString(QUuid::WithoutBraces).toStdString());
    query.bind(startIdx + 1, instance.name.toStdString());
    query.bind(startIdx + 2, coreTypeToString(instance.core_type).toStdString());
    query.bind(startIdx + 3, instance.socks_port);
    query.bind(startIdx + 4, instance.http_port);
    query.bind(startIdx + 5, instance.api_port);
    query.bind(startIdx + 6, instance.bind_address.toStdString());
    query.bind(startIdx + 7, instance.auto_start ? 1 : 0);
    query.bind(startIdx + 8, instance.restart_on_crash ? 1 : 0);
    query.bind(startIdx + 9, instance.restore_last_profile ? 1 : 0);
    query.bind(startIdx + 10, instance.startup_delay_ms);
    query.bind(startIdx + 11, instance.last_profile_id);
    query.bind(startIdx + 12, instance.last_active_profile_id);
    
    QJsonDocument dnsDoc(instance.dns_settings.ToJson());
    query.bind(startIdx + 13, QString::fromUtf8(dnsDoc.toJson(QJsonDocument::Compact)).toStdString());
    
    QJsonDocument coreDoc(instance.core_config.ToJson());
    query.bind(startIdx + 14, QString::fromUtf8(coreDoc.toJson(QJsonDocument::Compact)).toStdString());
    
    query.bind(startIdx + 15, static_cast<int64_t>(instance.created_at));
    query.bind(startIdx + 16, static_cast<int64_t>(instance.updated_at));
}

std::optional<Instance> InstanceRepository::getById(const QUuid& id) const {
    if (id.isNull()) return std::nullopt;
    
    auto query = db.query("SELECT id, name, core_type, socks_port, http_port, api_port, "
                          "bind_address, auto_start, restart_on_crash, restore_last_profile, "
                          "startup_delay_ms, last_profile_id, last_active_profile_id, "
                          "dns_settings_json, core_config_json, created_at, updated_at "
                          "FROM instances WHERE id = ?", 
                          id.toString(QUuid::WithoutBraces).toStdString());
    
    if (query && query->executeStep()) {
        return *instanceFromRow(*query);
    }
    return std::nullopt;
}

std::optional<Instance> InstanceRepository::getByName(const QString& name) const {
    auto query = db.query("SELECT id, name, core_type, socks_port, http_port, api_port, "
                          "bind_address, auto_start, restart_on_crash, restore_last_profile, "
                          "startup_delay_ms, last_profile_id, last_active_profile_id, "
                          "dns_settings_json, core_config_json, created_at, updated_at "
                          "FROM instances WHERE name = ?", name.toStdString());
    
    if (query && query->executeStep()) {
        return *instanceFromRow(*query);
    }
    return std::nullopt;
}

std::vector<Instance> InstanceRepository::getAll() const {
    std::vector<Instance> instances;
    auto query = db.query("SELECT id, name, core_type, socks_port, http_port, api_port, "
                          "bind_address, auto_start, restart_on_crash, restore_last_profile, "
                          "startup_delay_ms, last_profile_id, last_active_profile_id, "
                          "dns_settings_json, core_config_json, created_at, updated_at "
                          "FROM instances");
    
    if (query) {
        while (query->executeStep()) {
            instances.push_back(*instanceFromRow(*query));
        }
    }
    return instances;
}

std::vector<Instance> InstanceRepository::getAllOrdered() const {
    std::vector<Instance> instances;
    auto query = db.query("SELECT id, name, core_type, socks_port, http_port, api_port, "
                          "bind_address, auto_start, restart_on_crash, restore_last_profile, "
                          "startup_delay_ms, last_profile_id, last_active_profile_id, "
                          "dns_settings_json, core_config_json, created_at, updated_at "
                          "FROM instances ORDER BY created_at ASC");
    
    if (query) {
        while (query->executeStep()) {
            instances.push_back(*instanceFromRow(*query));
        }
    }
    return instances;
}

bool InstanceRepository::save(const Instance& instance) {
    if (instance.id.isNull()) {
        LOG_ERROR("Cannot save Instance with null UUID");
        return false;
    }
    
    std::lock_guard<std::recursive_mutex> lock(db.WriteMutex());
    
    try {
        bool exists = this->exists(instance.id);
        
        if (exists) {
            db.execThrow(R"(
                UPDATE instances SET
                    name = ?, core_type = ?, socks_port = ?, http_port = ?, api_port = ?,
                    bind_address = ?, auto_start = ?, restart_on_crash = ?, restore_last_profile = ?,
                    startup_delay_ms = ?, last_profile_id = ?, last_active_profile_id = ?,
                    dns_settings_json = ?, core_config_json = ?, updated_at = ?
                WHERE id = ?
            )",
                instance.name.toStdString(),
                coreTypeToString(instance.core_type).toStdString(),
                instance.socks_port, instance.http_port, instance.api_port,
                instance.bind_address.toStdString(),
                instance.auto_start ? 1 : 0,
                instance.restart_on_crash ? 1 : 0,
                instance.restore_last_profile ? 1 : 0,
                instance.startup_delay_ms,
                instance.last_profile_id,
                instance.last_active_profile_id,
                QString::fromUtf8(QJsonDocument(instance.dns_settings.ToJson()).toJson(QJsonDocument::Compact)).toStdString(),
                QString::fromUtf8(QJsonDocument(instance.core_config.ToJson()).toJson(QJsonDocument::Compact)).toStdString(),
                instance.updated_at,
                instance.id.toString(QUuid::WithoutBraces).toStdString()
            );
        } else {
            db.execThrow(R"(
                INSERT INTO instances (
                    id, name, core_type, socks_port, http_port, api_port,
                    bind_address, auto_start, restart_on_crash, restore_last_profile,
                    startup_delay_ms, last_profile_id, last_active_profile_id,
                    dns_settings_json, core_config_json, created_at, updated_at
                ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
            )",
                instance.id.toString(QUuid::WithoutBraces).toStdString(),
                instance.name.toStdString(),
                coreTypeToString(instance.core_type).toStdString(),
                instance.socks_port, instance.http_port, instance.api_port,
                instance.bind_address.toStdString(),
                instance.auto_start ? 1 : 0,
                instance.restart_on_crash ? 1 : 0,
                instance.restore_last_profile ? 1 : 0,
                instance.startup_delay_ms,
                instance.last_profile_id,
                instance.last_active_profile_id,
                QString::fromUtf8(QJsonDocument(instance.dns_settings.ToJson()).toJson(QJsonDocument::Compact)).toStdString(),
                QString::fromUtf8(QJsonDocument(instance.core_config.ToJson()).toJson(QJsonDocument::Compact)).toStdString(),
                instance.created_at,
                instance.updated_at
            );
        }
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(QString("Failed to save Instance %1: %2").arg(instance.id.toString()).arg(e.what()));
        return false;
    }
}

bool InstanceRepository::remove(const QUuid& id) {
    if (id.isNull()) return false;
    
    std::lock_guard<std::recursive_mutex> lock(db.WriteMutex());
    
    try {
        db.execThrow("DELETE FROM instances WHERE id = ?", id.toString(QUuid::WithoutBraces).toStdString());
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(QString("Failed to remove Instance %1: %2").arg(id.toString()).arg(e.what()));
        return false;
    }
}

bool InstanceRepository::exists(const QUuid& id) const {
    if (id.isNull()) return false;
    
    auto query = db.query("SELECT 1 FROM instances WHERE id = ?", id.toString(QUuid::WithoutBraces).toStdString());
    return query && query->executeStep();
}

int InstanceRepository::count() const {
    auto query = db.query("SELECT COUNT(*) FROM instances");
    if (query && query->executeStep()) {
        return query->getColumn(0).getInt();
    }
    return 0;
}

std::optional<Instance> InstanceRepository::getDefaultInstance() const {
    auto query = db.query("SELECT id, name, core_type, socks_port, http_port, api_port, "
                          "bind_address, auto_start, restart_on_crash, restore_last_profile, "
                          "startup_delay_ms, last_profile_id, last_active_profile_id, "
                          "dns_settings_json, core_config_json, created_at, updated_at "
                          "FROM instances ORDER BY created_at ASC LIMIT 1");
    
    if (query && query->executeStep()) {
        return *instanceFromRow(*query);
    }
    return std::nullopt;
}

bool InstanceRepository::setDefaultInstance(const QUuid& id) {
    return exists(id);
}

std::vector<QUuid> InstanceRepository::getAutoStartInstances() const {
    std::vector<QUuid> uuids;
    auto query = db.query("SELECT id FROM instances WHERE auto_start = 1 ORDER BY startup_delay_ms ASC, created_at ASC");
    
    if (query) {
        while (query->executeStep()) {
            QUuid uuid = QUuid::fromString(QString::fromStdString(query->getColumn(0).getText()));
            if (!uuid.isNull()) {
                uuids.push_back(uuid);
            }
        }
    }
    return uuids;
}

} // namespace Configs