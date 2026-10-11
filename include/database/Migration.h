#pragma once

#include "database/Database.h"
#include "database/entities/Instance.h"
#include <QUuid>
#include <string>
#include <optional>

namespace Configs {

struct MigrationResult {
    bool migrated = false;
    std::string defaultInstanceId;
    int groupsMigrated = 0;
    int settingsMigrated = 0;
    std::string error;
};

MigrationResult runThroneToCoreDeckMigration(Database& db, const std::string& dbPath);

bool isMigrationNeeded(Database& db);
std::string getMigrationVersion(Database& db);
void setMigrationVersion(Database& db, const std::string& version);

} // namespace Configs
