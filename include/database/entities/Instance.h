#pragma once

#include <QDateTime>
#include <QJsonObject>
#include <QList>
#include <QUuid>
#include <QString>
#include <optional>

namespace Configs
{

enum class CoreType : int {
    Xray = 0,
    SingBox = 1,
    External = 2
};

QString coreTypeToString(CoreType type);
CoreType coreTypeFromString(const QString& str);

struct InstanceDNSSettings {
    QString remote_dns;
    QString direct_dns;
    QString dns_object;
    bool dns_disable_cache = false;
    bool dns_disable_expire = false;
    bool dns_persist_cache = false;
    bool dns_reverse_mapping = false;
    bool dns_optimistic = false;
    bool dns_use_hosts = false;
    bool dns_predefined_enable = false;
    QStringList dns_predefined_rules;
    int dns_cache_capacity = 0;
    int dns_optimistic_timeout = 0;
    int dns_query_timeout = 0;
    QString dns_final_out;
    QString domain_strategy;
    QString outbound_domain_strategy;

    [[nodiscard]] QJsonObject ToJson() const;
    static InstanceDNSSettings FromJson(const QJsonObject& json);
};

struct InstanceCoreConfig {
    QString core_path;
    QString core_args;
    QString core_config_json;

    [[nodiscard]] QJsonObject ToJson() const;
    static InstanceCoreConfig FromJson(const QJsonObject& json);
};

class Instance {
public:
    QUuid id;
    QString name;
    bool enabled = true;
    CoreType core_type = CoreType::Xray;
    InstanceCoreConfig core_config;
    int socks_port = 10808;
    int http_port = 10809;
    int api_port = -1;
    QString bind_address = "127.0.0.1";
    bool auto_start = false;
    bool restart_on_crash = false;
    bool restore_last_profile = true;
    int startup_delay_ms = 0;
    int last_profile_id = -1;
    int last_active_profile_id = -1;
    InstanceDNSSettings dns_settings;
    qint64 created_at = 0;
    qint64 updated_at = 0;

    Instance() = default;
    explicit Instance(const QUuid& uuid);

    static Instance CreateDefault();

    [[nodiscard]] QJsonObject ToJson() const;
    static Instance FromJson(const QJsonObject& json);

    [[nodiscard]] bool IsValid() const;
    [[nodiscard]] QString Validate() const;

    void Touch();

    bool operator==(const Instance& other) const { return id == other.id; }
    bool operator!=(const Instance& other) const { return id != other.id; }
};

using InstancePtr = std::shared_ptr<Instance>;

} // namespace Configs