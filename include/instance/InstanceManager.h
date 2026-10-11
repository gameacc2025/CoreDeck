#pragma once

#include "database/entities/Instance.h"
#include "database/InstanceRepository.h"
#include <QUuid>
#include <optional>
#include <vector>
#include <mutex>

namespace Configs {

class DatabaseManager;

class InstanceManager {
public:
    static InstanceManager& instance();

    ~InstanceManager();

    // Deleted copy/move
    InstanceManager(const InstanceManager&) = delete;
    InstanceManager& operator=(const InstanceManager&) = delete;
    InstanceManager(InstanceManager&&) = delete;
    InstanceManager& operator=(InstanceManager&&) = delete;

    // Initialize with DatabaseManager (must be called before any other method)
    void initialize(DatabaseManager* dbManager);

    // Instance lifecycle
    std::optional<Instance> createInstance(const QString& name, CoreType coreType = CoreType::Xray);
    std::optional<Instance> getInstance(const QUuid& id) const;
    std::optional<Instance> getInstanceByName(const QString& name) const;
    std::vector<Instance> getAllInstances() const;
    std::vector<Instance> getAllInstancesOrdered() const;
    bool updateInstance(const Instance& instance);
    bool deleteInstance(const QUuid& id);
    bool instanceExists(const QUuid& id) const;
    int instanceCount() const;

    // Active Instance management
    std::optional<Instance> getActiveInstance() const;
    bool setActiveInstance(const QUuid& id);
    bool clearActiveInstance();

    // Default Instance
    std::optional<Instance> getDefaultInstance() const;
    bool ensureDefaultInstanceExists();

    // Auto-start
    std::vector<QUuid> getAutoStartInstanceIds() const;

    // Utility
    bool isInitialized() const { return initialized_; }

private:
    InstanceManager() = default;
    
    void loadInstances();
    void saveActiveInstanceId(const QUuid& id);
    std::optional<QUuid> loadActiveInstanceId() const;

    DatabaseManager* dbManager_ = nullptr;
    InstanceRepository* instanceRepo_ = nullptr;
    mutable std::mutex mutex_;
    QUuid activeInstanceId_;
    bool initialized_ = false;
    bool instancesLoaded_ = false;
    std::vector<Instance> cachedInstances_;
};

} // namespace Configs
