#pragma once

#include "database/Database.h"
#include "database/entities/Instance.h"
#include <QUuid>
#include <optional>
#include <vector>

namespace Configs {

class InstanceRepository {
private:
    Database& db;
    
    std::shared_ptr<Instance> instanceFromRow(SQLite::Statement& query) const;
    void bindInstance(SQLite::Statement& query, const Instance& instance, int startIdx = 1) const;

public:
    explicit InstanceRepository(Database& database);
    
    void createTables();
    
    std::optional<Instance> getById(const QUuid& id) const;
    std::optional<Instance> getByName(const QString& name) const;
    std::vector<Instance> getAll() const;
    std::vector<Instance> getAllOrdered() const;
    
    bool save(const Instance& instance);
    bool remove(const QUuid& id);
    
    bool exists(const QUuid& id) const;
    int count() const;
    
    std::optional<Instance> getDefaultInstance() const;
    bool setDefaultInstance(const QUuid& id);
    
    std::vector<QUuid> getAutoStartInstances() const;
};

} // namespace Configs
