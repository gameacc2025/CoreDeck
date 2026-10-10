#include "include/instance/InstanceManager.h"
#include "include/database/DatabaseManager.h"
#include "include/database/InstanceRepository.h"
#include "include/global/Logger.hpp"
#include "include/global/Utils.hpp"
#include <QUuid>
#include <QDateTime>
#include <QSettings>
#include <algorithm>

namespace Configs {

InstanceManager& InstanceManager::instance() {
    static InstanceManager manager;
    return manager;
}

InstanceManager::~InstanceManager() = default;

void InstanceManager::initialize(DatabaseManager* dbManager) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (initialized_) {
        LOG_WARN("InstanceManager already initialized");
        return;
    }
    
    if (!dbManager) {
        LOG_ERROR("InstanceManager initialization failed: null DatabaseManager");
        return;
    }
    
    dbManager_ = dbManager;
    instanceRepo_ = &dbManager_->instanceRepo;
    
    loadInstances();
    ensureDefaultInstanceExists();
    
    // Load active instance from settings
    auto activeId = loadActiveInstanceId();
    if (activeId && instanceExists(*activeId)) {
        activeInstanceId_ = *activeId;
    } else if (!cachedInstances_.empty()) {
        activeInstanceId_ = cachedInstances_.front().id;
        saveActiveInstanceId(activeInstanceId_);
    }
    
    initialized_ = true;
    LOG_INFO(QString("InstanceManager initialized with %1 instance(s), active: %2")
        .arg(cachedInstances_.size())
        .arg(activeInstanceId_.isNull() ? "none" : activeInstanceId_.toString()));
}

void InstanceManager::loadInstances() {
    if (!instanceRepo_) return;
    
    cachedInstances_ = instanceRepo_->getAllOrdered();
    instancesLoaded_ = true;
}

std::optional<QUuid> InstanceManager::loadActiveInstanceId() const {
    QSettings settings("CoreDeck", "CoreDeck");
    QString idStr = settings.value("active_instance_id").toString();
    if (idStr.isEmpty()) return std::nullopt;
    
    QUuid uuid = QUuid::fromString(idStr);
    return uuid.isNull() ? std::nullopt : std::make_optional(uuid);
}

void InstanceManager::saveActiveInstanceId(const QUuid& id) {
    QSettings settings("CoreDeck", "CoreDeck");
    settings.setValue("active_instance_id", id.toString(QUuid::WithoutBraces));
    activeInstanceId_ = id;
}

bool InstanceManager::ensureDefaultInstanceExists() {
    if (!instanceRepo_) return false;
    
    auto defaultInstance = instanceRepo_->getDefaultInstance();
    if (defaultInstance) {
        return true;
    }
    
    LOG_INFO("Creating Default Instance on first run...");
    return createInstance("Default", CoreType::Xray).has_value();
}

std::optional<Instance> InstanceManager::createInstance(const QString& name, CoreType coreType) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!instanceRepo_ || !initialized_) {
        LOG_ERROR("InstanceManager not initialized");
        return std::nullopt;
    }
    
    // Check for duplicate name
    if (getInstanceByName(name)) {
        LOG_WARN(QString("Instance with name '%1' already exists").arg(name));
        return std::nullopt;
    }
    
    Instance instance = Instance::CreateDefault();
    instance.name = name;
    instance.core_type = coreType;
    instance.Touch();
    
    if (!instanceRepo_->save(instance)) {
        LOG_ERROR(QString("Failed to save new Instance: %1").arg(name));
        return std::nullopt;
    }
    
    // Reload cache
    loadInstances();
    
    LOG_INFO(QString("Created Instance: %1 (%2)").arg(name).arg(instance.id.toString()));
    return instance;
}

std::optional<Instance> InstanceManager::getInstance(const QUuid& id) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!instanceRepo_) return std::nullopt;
    
    // Check cache first
    for (const auto& inst : cachedInstances_) {
        if (inst.id == id) return inst;
    }
    
    // Fallback to repository
    return instanceRepo_->getById(id);
}

std::optional<Instance> InstanceManager::getInstanceByName(const QString& name) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!instanceRepo_) return std::nullopt;
    
    // Check cache first
    for (const auto& inst : cachedInstances_) {
        if (inst.name == name) return inst;
    }
    
    // Fallback to repository
    return instanceRepo_->getByName(name);
}

std::vector<Instance> InstanceManager::getAllInstances() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return cachedInstances_;
}

std::vector<Instance> InstanceManager::getAllInstancesOrdered() const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!instancesLoaded_) {
        const_cast<InstanceManager*>(this)->loadInstances();
    }
    return cachedInstances_;
}

bool InstanceManager::updateInstance(const Instance& instance) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!instanceRepo_ || !initialized_) {
        LOG_ERROR("InstanceManager not initialized");
        return false;
    }
    
    if (instance.id.isNull()) {
        LOG_ERROR("Cannot update Instance with null UUID");
        return false;
    }
    
    // Check if another instance has this name
    auto existing = instanceRepo_->getByName(instance.name);
    if (existing && existing->id != instance.id) {
        LOG_WARN(QString("Another Instance already has name '%1'").arg(instance.name));
        return false;
    }
    
    Instance mutableInstance = instance;
    mutableInstance.Touch();
    
    if (!instanceRepo_->save(mutableInstance)) {
        LOG_ERROR(QString("Failed to update Instance: %1").arg(instance.id.toString()));
        return false;
    }
    
    // Update cache
    loadInstances();
    
    // If this is the active instance, update its reference
    if (activeInstanceId_ == instance.id) {
        activeInstanceId_ = mutableInstance.id;
    }
    
    LOG_INFO(QString("Updated Instance: %1").arg(instance.id.toString()));
    return true;
}

bool InstanceManager::deleteInstance(const QUuid& id) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!instanceRepo_ || !initialized_) {
        LOG_ERROR("InstanceManager not initialized");
        return false;
    }
    
    if (id.isNull()) return false;
    
    // Prevent deleting the last instance
    if (cachedInstances_.size() <= 1) {
        LOG_WARN("Cannot delete the last Instance");
        return false;
    }
    
    // If deleting active instance, clear active
    if (activeInstanceId_ == id) {
        clearActiveInstance();
    }
    
    if (!instanceRepo_->remove(id)) {
        LOG_ERROR(QString("Failed to delete Instance: %1").arg(id.toString()));
        return false;
    }
    
    // Reload cache
    loadInstances();
    
    // Set new active instance if none
    if (activeInstanceId_.isNull() && !cachedInstances_.empty()) {
        activeInstanceId_ = cachedInstances_.front().id;
        saveActiveInstanceId(activeInstanceId_);
    }
    
    LOG_INFO(QString("Deleted Instance: %1").arg(id.toString()));
    return true;
}

bool InstanceManager::instanceExists(const QUuid& id) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!instanceRepo_) return false;
    return instanceRepo_->exists(id);
}

int InstanceManager::instanceCount() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return static_cast<int>(cachedInstances_.size());
}

std::optional<Instance> InstanceManager::getActiveInstance() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (activeInstanceId_.isNull()) return std::nullopt;
    return getInstance(activeInstanceId_);
}

bool InstanceManager::setActiveInstance(const QUuid& id) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!instanceExists(id)) {
        LOG_WARN(QString("Cannot set active Instance: %1 not found").arg(id.toString()));
        return false;
    }
    
    activeInstanceId_ = id;
    saveActiveInstanceId(id);
    
    LOG_INFO(QString("Active Instance set to: %1").arg(id.toString()));
    return true;
}

bool InstanceManager::clearActiveInstance() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    activeInstanceId_ = QUuid();
    QSettings settings("CoreDeck", "CoreDeck");
    settings.remove("active_instance_id");
    
    // Set to first available instance
    if (!cachedInstances_.empty()) {
        activeInstanceId_ = cachedInstances_.front().id;
        saveActiveInstanceId(activeInstanceId_);
    }
    
    return true;
}

std::optional<Instance> InstanceManager::getDefaultInstance() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!instanceRepo_) return std::nullopt;
    return instanceRepo_->getDefaultInstance();
}

std::vector<QUuid> InstanceManager::getAutoStartInstanceIds() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!instanceRepo_) return {};
    return instanceRepo_->getAutoStartInstances();
}

} // namespace Configs