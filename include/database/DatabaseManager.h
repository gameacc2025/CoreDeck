#pragma once

#include "include/database/Database.h"
#include "include/database/SettingsRepo.h"
#include "include/database/Migration.h"
#include <string>
#include <memory>

namespace Configs {
    class RoutesRepo;
    class GroupsRepo;
    class ProfilesRepo;
    class OtpProfilesRepo;
    class IpListsRepo;
    class IpScansRepo;
    class TrafficStatsRepo;
    class MarkersRepo;
    class InstanceRepository;

    class DatabaseManager {
    private:
        Database db;
        Database statsDb;
        Database scanDb;

        static void createEntityIdsTable(Database& db);
        static bool entityIdsColumnExists(Database& db, const char* columnName);
        static std::string deriveStatsDbPath(const std::string& dbPath);
        static std::string prepareStatsDb(const std::string& path);
        static QString statsDbUnusableReason(const std::string& path);
        static void quarantineDbFile(const std::string& path);
        void initializeRepos();
        void applyMigrations();
        void runCoreDeckMigration();

    public:
        std::unique_ptr<ProfilesRepo> profilesRepo;
        std::unique_ptr<GroupsRepo> groupsRepo;
        std::unique_ptr<RoutesRepo> routesRepo;
        std::unique_ptr<OtpProfilesRepo> otpProfilesRepo;
        std::unique_ptr<SettingsRepo> settingsRepo;
        std::unique_ptr<TrafficStatsRepo> trafficStatsRepo;
        std::unique_ptr<MarkersRepo> markersRepo;
        std::unique_ptr<IpListsRepo> ipListsRepo;
        std::unique_ptr<IpScansRepo> ipScansRepo;
        std::unique_ptr<InstanceRepository> instanceRepo;

        explicit DatabaseManager(const std::string& dbPath);
        ~DatabaseManager() = default;

        void RunDeferredMaintenance();

        DatabaseManager(const DatabaseManager&) = delete;
        DatabaseManager& operator=(const DatabaseManager&) = delete;

        Database& getDatabase() { return db; }
        const Database& getDatabase() const { return db; }
    };

    inline DatabaseManager* dataManager;
}