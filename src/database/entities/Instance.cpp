#include "include/database/entities/Instance.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QDateTime>
#include <QUuid>

namespace Configs
{

QString coreTypeToString(CoreType type)
{
    switch (type) {
        case CoreType::Xray: return "xray";
        case CoreType::SingBox: return "singbox";
        case CoreType::External: return "external";
    }
    return "xray";
}

CoreType coreTypeFromString(const QString& str)
{
    const QString lower = str.toLower();
    if (lower == "xray") return CoreType::Xray;
    if (lower == "singbox" || lower == "sing-box") return CoreType::SingBox;
    if (lower == "external") return CoreType::External;
    return CoreType::Xray;
}

QJsonObject InstanceDNSSettings::ToJson() const
{
    QJsonObject json;
    json["remote_dns"] = remote_dns;
    json["direct_dns"] = direct_dns;
    json["dns_object"] = dns_object;
    json["dns_disable_cache"] = dns_disable_cache;
    json["dns_disable_expire"] = dns_disable_expire;
    json["dns_persist_cache"] = dns_persist_cache;
    json["dns_reverse_mapping"] = dns_reverse_mapping;
    json["dns_optimistic"] = dns_optimistic;
    json["dns_use_hosts"] = dns_use_hosts;
    json["dns_predefined_enable"] = dns_predefined_enable;
    json["dns_predefined_rules"] = QJsonArray::fromStringList(dns_predefined_rules);
    json["dns_cache_capacity"] = dns_cache_capacity;
    json["dns_optimistic_timeout"] = dns_optimistic_timeout;
    json["dns_query_timeout"] = dns_query_timeout;
    json["dns_final_out"] = dns_final_out;
    json["domain_strategy"] = domain_strategy;
    json["outbound_domain_strategy"] = outbound_domain_strategy;
    return json;
}

InstanceDNSSettings InstanceDNSSettings::FromJson(const QJsonObject& json)
{
    InstanceDNSSettings settings;
    settings.remote_dns = json["remote_dns"].toString();
    settings.direct_dns = json["direct_dns"].toString();
    settings.dns_object = json["dns_object"].toString();
    settings.dns_disable_cache = json["dns_disable_cache"].toBool();
    settings.dns_disable_expire = json["dns_disable_expire"].toBool();
    settings.dns_persist_cache = json["dns_persist_cache"].toBool();
    settings.dns_reverse_mapping = json["dns_reverse_mapping"].toBool();
    settings.dns_optimistic = json["dns_optimistic"].toBool();
    settings.dns_use_hosts = json["dns_use_hosts"].toBool();
    settings.dns_predefined_enable = json["dns_predefined_enable"].toBool();
    if (json.contains("dns_predefined_rules") && json["dns_predefined_rules"].isArray()) {
        for (const auto& v : json["dns_predefined_rules"].toArray()) {
            settings.dns_predefined_rules << v.toString();
        }
    }
    settings.dns_cache_capacity = json["dns_cache_capacity"].toInt();
    settings.dns_optimistic_timeout = json["dns_optimistic_timeout"].toInt();
    settings.dns_query_timeout = json["dns_query_timeout"].toInt();
    settings.dns_final_out = json["dns_final_out"].toString();
    settings.domain_strategy = json["domain_strategy"].toString();
    settings.outbound_domain_strategy = json["outbound_domain_strategy"].toString();
    return settings;
}

QJsonObject InstanceCoreConfig::ToJson() const
{
    QJsonObject json;
    json["core_path"] = core_path;
    json["core_args"] = core_args;
    json["core_config_json"] = core_config_json;
    return json;
}

InstanceCoreConfig InstanceCoreConfig::FromJson(const QJsonObject& json)
{
    InstanceCoreConfig config;
    config.core_path = json["core_path"].toString();
    config.core_args = json["core_args"].toString();
    config.core_config_json = json["core_config_json"].toString();
    return config;
}

Instance::Instance(const QUuid& uuid) : id(uuid)
{
    Touch();
}

Instance Instance::CreateDefault()
{
    Instance instance;
    instance.id = QUuid::createUuid();
    instance.name = "Default";
    instance.enabled = true;
    instance.core_type = CoreType::Xray;
    instance.socks_port = 10808;
    instance.http_port = 10809;
    instance.api_port = -1;
    instance.bind_address = "127.0.0.1";
    instance.auto_start = false;
    instance.restart_on_crash = false;
    instance.restore_last_profile = true;
    instance.startup_delay_ms = 0;
    instance.Touch();
    return instance;
}

QJsonObject Instance::ToJson() const
{
    QJsonObject json;
    json["id"] = id.toString(QUuid::WithoutBraces);
    json["name"] = name;
    json["enabled"] = enabled;
    json["core_type"] = coreTypeToString(core_type);
    json["core_config"] = core_config.ToJson();
    json["socks_port"] = socks_port;
    json["http_port"] = http_port;
    json["api_port"] = api_port;
    json["bind_address"] = bind_address;
    json["auto_start"] = auto_start;
    json["restart_on_crash"] = restart_on_crash;
    json["restore_last_profile"] = restore_last_profile;
    json["startup_delay_ms"] = startup_delay_ms;
    json["last_profile_id"] = last_profile_id;
    json["last_active_profile_id"] = last_active_profile_id;
    json["dns_settings"] = dns_settings.ToJson();
    json["created_at"] = static_cast<qint64>(created_at);
    json["updated_at"] = static_cast<qint64>(updated_at);
    return json;
}

Instance Instance::FromJson(const QJsonObject& json)
{
    Instance instance;
    instance.id = QUuid(json["id"].toString());
    instance.name = json["name"].toString();
    instance.enabled = json["enabled"].toBool();
    instance.core_type = coreTypeFromString(json["core_type"].toString());
    if (json.contains("core_config") && json["core_config"].isObject()) {
        instance.core_config = InstanceCoreConfig::FromJson(json["core_config"].toObject());
    }
    instance.socks_port = json["socks_port"].toInt(10808);
    instance.http_port = json["http_port"].toInt(10809);
    instance.api_port = json["api_port"].toInt(-1);
    instance.bind_address = json["bind_address"].toString("127.0.0.1");
    instance.auto_start = json["auto_start"].toBool();
    instance.restart_on_crash = json["restart_on_crash"].toBool();
    instance.restore_last_profile = json["restore_last_profile"].toBool(true);
    instance.startup_delay_ms = json["startup_delay_ms"].toInt();
    instance.last_profile_id = json["last_profile_id"].toInt(-1);
    instance.last_active_profile_id = json["last_active_profile_id"].toInt(-1);
    if (json.contains("dns_settings") && json["dns_settings"].isObject()) {
        instance.dns_settings = InstanceDNSSettings::FromJson(json["dns_settings"].toObject());
    }
    instance.created_at = json["created_at"].toVariant().toLongLong();
    instance.updated_at = json["updated_at"].toVariant().toLongLong();
    return instance;
}

bool Instance::IsValid() const
{
    return !id.isNull() && !name.isEmpty() && socks_port > 0 && http_port > 0;
}

QString Instance::Validate() const
{
    if (id.isNull()) return "Instance ID is null";
    if (name.trimmed().isEmpty()) return "Instance name cannot be empty";
    if (socks_port <= 0 || socks_port > 65535) return "Invalid SOCKS port";
    if (http_port <= 0 || http_port > 65535) return "Invalid HTTP port";
    if (api_port != -1 && (api_port <= 0 || api_port > 65535)) return "Invalid API port";
    if (bind_address.isEmpty()) return "Bind address cannot be empty";
    if (startup_delay_ms < 0) return "Startup delay cannot be negative";
    return {};
}

void Instance::Touch()
{
    const qint64 now = QDateTime::currentSecsSinceEpoch();
    if (created_at == 0) created_at = now;
    updated_at = now;
}

} // namespace Configs