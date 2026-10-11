#include "database/entities/Instance.h"

#include <QCoreApplication>
#include <QDebug>
#include <QUuid>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    int passed = 0;
    int failed = 0;

    auto check = [&](bool condition, const QString& testName) {
        if (condition) {
            qDebug() << "[PASS]" << testName;
            ++passed;
        } else {
            qDebug() << "[FAIL]" << testName;
            ++failed;
        }
    };

    // Test 1: Create a valid Instance
    {
        Configs::Instance instance = Configs::Instance::CreateDefault();
        check(instance.IsValid(), "CreateDefault produces valid instance");
        check(!instance.id.isNull(), "Instance has non-null UUID");
        check(instance.name == "Default", "Default name is 'Default'");
        check(instance.core_type == Configs::CoreType::Xray, "Default core type is Xray");
        check(instance.socks_port == 10808, "Default SOCKS port is 10808");
        check(instance.http_port == 10809, "Default HTTP port is 10809");
        check(instance.bind_address == "127.0.0.1", "Default bind address is 127.0.0.1");
        check(instance.auto_start == false, "Default auto_start is false");
        check(instance.restart_on_crash == false, "Default restart_on_crash is false");
        check(instance.restore_last_profile == true, "Default restore_last_profile is true");
        check(instance.created_at > 0, "Created timestamp is set");
        check(instance.updated_at > 0, "Updated timestamp is set");
    }

    // Test 2: Generate and preserve UUID
    {
        Configs::Instance instance = Configs::Instance::CreateDefault();
        QUuid originalId = instance.id;
        
        // Serialize and deserialize
        QJsonObject json = instance.ToJson();
        Configs::Instance restored = Configs::Instance::FromJson(json);
        
        check(restored.id == originalId, "UUID preserved through serialization");
        check(!restored.id.isNull(), "Restored UUID is not null");
    }

    // Test 3: Rename Instance without changing identity
    {
        Configs::Instance instance = Configs::Instance::CreateDefault();
        QUuid originalId = instance.id;
        
        instance.name = "Renamed Instance";
        instance.Touch();
        
        QJsonObject json = instance.ToJson();
        Configs::Instance restored = Configs::Instance::FromJson(json);
        
        check(restored.id == originalId, "Identity preserved after rename");
        check(restored.name == "Renamed Instance", "Name changed correctly");
        check(restored.updated_at >= instance.updated_at, "Updated timestamp updated");
    }

    // Test 4: Handle invalid/missing identity
    {
        Configs::Instance instance;
        check(!instance.IsValid(), "Default-constructed instance is invalid");
        
        QString error = instance.Validate();
        check(!error.isEmpty(), "Validate() returns error for invalid instance");
    }

    // Test 5: Validate supported Core types
    {
        Configs::Instance instance = Configs::Instance::CreateDefault();
        
        instance.core_type = Configs::CoreType::Xray;
        check(instance.Validate().isEmpty(), "Xray core type is valid");
        
        instance.core_type = Configs::CoreType::SingBox;
        check(instance.Validate().isEmpty(), "SingBox core type is valid");
        
        instance.core_type = Configs::CoreType::External;
        check(instance.Validate().isEmpty(), "External core type is valid");
    }

    // Test 6: Verify default Core selection
    {
        Configs::Instance instance = Configs::Instance::CreateDefault();
        check(instance.core_type == Configs::CoreType::Xray, "Default core type is Xray per DEC-007");
    }

    // Test 7: Validate relevant endpoint settings
    {
        Configs::Instance instance = Configs::Instance::CreateDefault();
        
        // Valid ports
        instance.socks_port = 10808;
        instance.http_port = 10809;
        check(instance.Validate().isEmpty(), "Valid ports pass validation");
        
        // Invalid SOCKS port
        instance.socks_port = 0;
        check(!instance.Validate().isEmpty(), "Zero SOCKS port fails validation");
        instance.socks_port = 70000;
        check(!instance.Validate().isEmpty(), "Out-of-range SOCKS port fails validation");
        
        // Invalid HTTP port
        instance.socks_port = 10808;
        instance.http_port = 0;
        check(!instance.Validate().isEmpty(), "Zero HTTP port fails validation");
        
        // Valid bind address
        instance.bind_address = "127.0.0.1";
        check(instance.Validate().isEmpty(), "Valid bind address passes");
        
        // Empty bind address
        instance.bind_address = "";
        check(!instance.Validate().isEmpty(), "Empty bind address fails validation");
    }

    // Test 8: Round-trip serialization and deserialization
    {
        Configs::Instance original = Configs::Instance::CreateDefault();
        original.name = "Test Instance";
        original.core_type = Configs::CoreType::SingBox;
        original.socks_port = 20808;
        original.http_port = 20809;
        original.api_port = 20810;
        original.bind_address = "127.0.0.1";
        original.auto_start = true;
        original.restart_on_crash = true;
        original.restore_last_profile = false;
        original.startup_delay_ms = 5000;
        original.last_profile_id = 42;
        original.last_active_profile_id = 43;
        original.dns_settings.remote_dns = "https://dns.example.com/dns-query";
        original.dns_settings.direct_dns = "1.1.1.1";
        original.dns_settings.dns_object = "{}";
        original.core_config.core_path = "/custom/xray";
        original.core_config.core_args = "-config /custom/config.json";
        original.core_config.core_config_json = "{}";
        original.Touch();
        
        QJsonObject json = original.ToJson();
        Configs::Instance restored = Configs::Instance::FromJson(json);
        
        check(restored.id == original.id, "Round-trip: UUID matches");
        check(restored.name == original.name, "Round-trip: name matches");
        check(restored.core_type == original.core_type, "Round-trip: core_type matches");
        check(restored.socks_port == original.socks_port, "Round-trip: socks_port matches");
        check(restored.http_port == original.http_port, "Round-trip: http_port matches");
        check(restored.api_port == original.api_port, "Round-trip: api_port matches");
        check(restored.bind_address == original.bind_address, "Round-trip: bind_address matches");
        check(restored.auto_start == original.auto_start, "Round-trip: auto_start matches");
        check(restored.restart_on_crash == original.restart_on_crash, "Round-trip: restart_on_crash matches");
        check(restored.restore_last_profile == original.restore_last_profile, "Round-trip: restore_last_profile matches");
        check(restored.startup_delay_ms == original.startup_delay_ms, "Round-trip: startup_delay_ms matches");
        check(restored.last_profile_id == original.last_profile_id, "Round-trip: last_profile_id matches");
        check(restored.last_active_profile_id == original.last_active_profile_id, "Round-trip: last_active_profile_id matches");
        check(restored.dns_settings.remote_dns == original.dns_settings.remote_dns, "Round-trip: dns remote_dns matches");
        check(restored.dns_settings.direct_dns == original.dns_settings.direct_dns, "Round-trip: dns direct_dns matches");
        check(restored.core_config.core_path == original.core_config.core_path, "Round-trip: core_path matches");
        check(restored.core_config.core_args == original.core_config.core_args, "Round-trip: core_args matches");
        check(restored.created_at == original.created_at, "Round-trip: created_at matches");
        check(restored.updated_at == original.updated_at, "Round-trip: updated_at matches");
    }

    // Test 9: Handle missing, malformed, or unsupported serialized fields safely
    {
        // Missing fields should use defaults
        QJsonObject minimal;
        minimal["id"] = QUuid::createUuid().toString(QUuid::WithoutBraces);
        minimal["name"] = "Minimal";
        
        Configs::Instance instance = Configs::Instance::FromJson(minimal);
        check(instance.id.toString() == minimal["id"].toString(), "Missing fields: ID parsed");
        check(instance.name == "Minimal", "Missing fields: name parsed");
        check(instance.core_type == Configs::CoreType::Xray, "Missing fields: core_type defaults to Xray");
        check(instance.socks_port == 10808, "Missing fields: socks_port defaults to 10808");
        check(instance.http_port == 10809, "Missing fields: http_port defaults to 10809");
        
        // Malformed JSON - invalid UUID
        QJsonObject badUuid;
        badUuid["id"] = "not-a-uuid";
        badUuid["name"] = "Bad";
        Configs::Instance badInstance = Configs::Instance::FromJson(badUuid);
        check(badInstance.id.isNull(), "Malformed UUID results in null UUID");
    }

    // Test 10: Equality based on UUID identity
    {
        Configs::Instance a = Configs::Instance::CreateDefault();
        Configs::Instance b = a;
        b.name = "Different";
        
        check(a == b, "Instances with same UUID are equal");
        check(!(a != b), "Instances with same UUID are not unequal");
        
        Configs::Instance c = Configs::Instance::CreateDefault();
        check(a != c, "Instances with different UUIDs are unequal");
    }

    // Test 11: coreTypeToString and coreTypeFromString
    {
        check(Configs::coreTypeToString(Configs::CoreType::Xray) == "xray", "Xray to string");
        check(Configs::coreTypeToString(Configs::CoreType::SingBox) == "singbox", "SingBox to string");
        check(Configs::coreTypeToString(Configs::CoreType::External) == "external", "External to string");
        
        check(Configs::coreTypeFromString("xray") == Configs::CoreType::Xray, "String to Xray");
        check(Configs::coreTypeFromString("singbox") == Configs::CoreType::SingBox, "String to SingBox");
        check(Configs::coreTypeFromString("SINGBOX") == Configs::CoreType::SingBox, "String to SingBox (case insensitive)");
        check(Configs::coreTypeFromString("sing-box") == Configs::CoreType::SingBox, "String to SingBox (with dash)");
        check(Configs::coreTypeFromString("external") == Configs::CoreType::External, "String to External");
        check(Configs::coreTypeFromString("unknown") == Configs::CoreType::Xray, "Unknown string defaults to Xray");
    }

    // Test 12: InstanceDNSSettings serialization
    {
        Configs::InstanceDNSSettings original;
        original.remote_dns = "https://dns.example.com";
        original.direct_dns = "8.8.8.8";
        original.dns_cache_capacity = 1000;
        
        QJsonObject json = original.ToJson();
        Configs::InstanceDNSSettings restored = Configs::InstanceDNSSettings::FromJson(json);
        
        check(restored.remote_dns == original.remote_dns, "DNSSettings: remote_dns round-trip");
        check(restored.direct_dns == original.direct_dns, "DNSSettings: direct_dns round-trip");
        check(restored.dns_cache_capacity == original.dns_cache_capacity, "DNSSettings: cache capacity round-trip");
    }

    // Test 13: InstanceCoreConfig serialization
    {
        Configs::InstanceCoreConfig original;
        original.core_path = "/usr/bin/xray";
        original.core_args = "-test";
        original.core_config_json = "{\"test\": true}";
        
        QJsonObject json = original.ToJson();
        Configs::InstanceCoreConfig restored = Configs::InstanceCoreConfig::FromJson(json);
        
        check(restored.core_path == original.core_path, "CoreConfig: core_path round-trip");
        check(restored.core_args == original.core_args, "CoreConfig: core_args round-trip");
        check(restored.core_config_json == original.core_config_json, "CoreConfig: core_config_json round-trip");
    }

    qDebug() << "\n=== Test Summary ===";
    qDebug() << "Passed:" << passed;
    qDebug() << "Failed:" << failed;
    qDebug() << "Total:" << (passed + failed);

    return failed > 0 ? 1 : 0;
}
