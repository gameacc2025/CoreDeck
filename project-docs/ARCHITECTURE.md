# CoreDeck Architecture

**Version:** 0.1 (Draft)  
**Status:** PLANNED  
**Last Updated:** 2026-10-10

---

## 1. Current Throne Architecture (Baseline)

### Technology Stack
| Layer | Technology |
|-------|------------|
| UI Framework | Qt 6 (Widgets) |
| Language | C++20 |
| Build System | CMake 3.20+ |
| Database | SQLite (via SQLiteCpp) |
| RPC | Local sockets + simple-protobuf (protobuf) |
| Core Engine | sing-box (primary), Xray-core (for Xray-specific configs) |
| Networking | Qt Network, OS native APIs |

### Key Components

```
Throne Application (Single Process)
├── main.cpp — Entry point, single-instance enforcement, config dir setup
├── Configs — Global settings repository (key-value in SQLite)
├── Database — SQLite schema + repositories (Groups, Profiles, Routes, Settings)
├── Config Generation — generate.cpp (sing-box JSON + Xray JSON)
├── Core Process — QProcess wrapper (CoreProcess.cpp)
│   ├── Starts core binary (sing-box/Xray)
│   ├── Sets up RPC socket (THRONE_CORE_SOCKET env var)
│   ├── Streams stdout/stderr to log
│   └── Auto-restart on crash
├── RPC Client — API::Client (LocalSocketChannel)
│   ├── Start/Stop core with config
│   ├── Query stats, connections, tests
│   └── Rule set updates, diagnostics
├── UI — MainWindow + dialogs (Qt Designer .ui + C++)
│   ├── Group/Profile management
│   ├── Connection control (start/stop profile)
│   ├── System proxy / TUN toggles
│   ├── Settings panels
│   └── Log viewer
└── Platform Abstraction — sys/{windows,linux,macos}/
    ├── Process management
    ├── System proxy
    ├── TUN interface
    ├── Kill switch
    └── Auto-elevation (Windows)
```

### Data Model (Current)

```
Settings (key-value table)
Groups
  └── Profiles (via Group.profiles list)
  └── SubscriptionOptions
Profiles
  └── outbound (polymorphic: vmess, vless, trojan, shadowsocks, chain, custom, extracore, etc.)
Routes
  └── RouteProfile, RouteRule
IpLists, IpScans, OtpProfiles, TrafficStats
```

### Core Execution Flow (Current)

```
User clicks "Start" on Profile
    ↓
MainWindow::profile_start()
    ↓
Configs::GenerateConfig(profile, group, routes, dns, ...)
    → Builds sing-box JSON config (with Xray bridge if needed)
    ↓
API::Client::Start(LoadConfigReq{core_config, xray_config})
    ↓
CoreProcess::Start() → QProcess::start(core_binary)
    ↓
Core binary reads THRONE_CORE_SOCKET, binds local socket
    ↓
RPC handshake → sing-box/Xray running
    ↓
CoreProcess::stateChanged(NotRunning) → auto-restart logic
```

### Single-Instance Enforcement
- `QLocalServer` with name derived from config directory hash
- Second instance connects, sends deeplink/files, exits
- Prevents multiple GUI processes

---

## 2. Target CoreDeck Architecture

### High-Level Structure

```
CoreDeck Application (Single Process)
├── InstanceManager (NEW)
│   ├── InstanceRegistry — CRUD for Instances, persistence
│   ├── ProcessSupervisor — Per-Instance CoreProcess management
│   ├── PortAllocator — Central port tracking & allocation
│   ├── HealthMonitor — Per-Instance status, crash detection, auto-restart
│   └── StateStore — Instance-level settings persistence
│
├── CoreAdapters (NEW — Strategy Pattern)
│   ├── CoreAdapter (Abstract Base)
│   ├── XrayAdapter — Xray-core specifics
│   ├── SingBoxAdapter — sing-box specifics
│   └── ExternalCoreAdapter — Generic external binary
│
├── RoutingManager (FUTURE — Phase 9+)
│   ├── LocalProxyPorts — Direct SOCKS/HTTP per Instance (Phase 1)
│   └── CentralRouter — Rule-based routing (Phase 9+)
│
├── Config Generation (ENHANCED)
│   ├── Per-Instance config generation
│   ├── Core-specific config builders
│   └── Chain/Core selection logic
│
├── Database (EXTENDED)
│   ├── Instance table (NEW)
│   ├── Group.instance_id FK (NEW)
│   ├── Profile.instance_id via Group (NEW)
│   ├── Instance-specific settings (NEW)
│   └── Migration from Throne single-instance schema
│
├── RPC Layer (EXTENDED)
│   ├── Multi-socket support (one per running Instance)
│   ├── Instance-scoped API calls
│   └── Connection pooling
│
└── UI (ENHANCED)
    ├── Instance selector (toolbar/sidebar)
    ├── Per-Instance profile/group views
    ├── Instance dashboard (status, ports, logs)
    └── Instance settings dialog
```

### New Database Schema

```sql
-- Instances table (NEW)
CREATE TABLE instances (
    id TEXT PRIMARY KEY,                    -- UUID
    name TEXT NOT NULL,
    enabled INTEGER NOT NULL DEFAULT 1,
    core_type TEXT NOT NULL DEFAULT 'xray', -- 'xray', 'singbox', 'external'
    core_path TEXT,
    core_args TEXT,
    core_config TEXT,                       -- JSON overrides
    socks_port INTEGER NOT NULL,
    http_port INTEGER NOT NULL,
    api_port INTEGER,
    bind_address TEXT NOT NULL DEFAULT '127.0.0.1',
    auto_start INTEGER NOT NULL DEFAULT 0,
    restart_on_crash INTEGER NOT NULL DEFAULT 0,
    restore_last_profile INTEGER NOT NULL DEFAULT 1,
    startup_delay_ms INTEGER NOT NULL DEFAULT 0,
    last_profile_id INTEGER,
    last_active_profile_id INTEGER,
    dns_settings TEXT,                      -- JSON
    created_at INTEGER NOT NULL,            -- Unix timestamp
    updated_at INTEGER NOT NULL
);

-- Groups: ADD instance_id FK
ALTER TABLE groups ADD COLUMN instance_id TEXT NOT NULL DEFAULT 'default-instance-uuid';
CREATE INDEX idx_groups_instance ON groups(instance_id);

-- Settings: Instance-scoped settings (NEW table)
CREATE TABLE instance_settings (
    instance_id TEXT NOT NULL,
    key TEXT NOT NULL,
    value TEXT NOT NULL,
    PRIMARY KEY (instance_id, key),
    FOREIGN KEY (instance_id) REFERENCES instances(id) ON DELETE CASCADE
);

-- Port allocations (NEW table for tracking)
CREATE TABLE port_allocations (
    instance_id TEXT NOT NULL,
    port_type TEXT NOT NULL,  -- 'socks', 'http', 'api'
    port INTEGER NOT NULL,
    PRIMARY KEY (instance_id, port_type),
    FOREIGN KEY (instance_id) REFERENCES instances(id) ON DELETE CASCADE
);
```

### Instance Lifecycle

```
Application Start
    ↓
InstanceManager::initialize()
    ├── Load all Instances from DB
    ├── PortAllocator::reserveAll(instance.ports)
    ├── For each Instance where enabled && auto_start:
    │   └── ProcessSupervisor::startInstance(instance)
    │       ├── CoreAdapter::generateConfig(instance, lastProfile)
    │       ├── CoreAdapter::start(instance, config)
    │       │   ├── Create unique RPC socket name
    │       │   ├── Launch Core process (QProcess)
    │       │   ├── Wait for RPC connection
    │       │   └── Register RPC client in pool
    │       └── HealthMonitor::register(instance)
    └── UI: Populate Instance selector, show Default Instance
```

### Core Adapter Pattern

```cpp
// Abstract interface
class CoreAdapter {
public:
    virtual ~CoreAdapter() = default;
    
    // Capabilities
    virtual QString name() const = 0;                    // "xray", "singbox"
    virtual QString version() const = 0;
    virtual bool supportsProtocol(Protocol) const = 0;
    virtual bool supportsChain() const = 0;
    virtual bool supportsTUN() const = 0;
    
    // Config
    virtual bool validateConfig(const Instance&, const Profile&, QString* error) = 0;
    virtual Config generateConfig(const Instance&, const Profile&) = 0;
    
    // Process lifecycle
    virtual ProcessResult start(const Instance&, const Config&) = 0;
    virtual void stop(const Instance&) = 0;
    virtual void restart(const Instance&) = 0;
    
    // Status & monitoring
    virtual InstanceStatus getStatus(const Instance&) = 0;
    virtual Logs getLogs(const Instance&) = 0;
    virtual Metrics getMetrics(const Instance&) = 0;
    
    // RPC
    virtual std::unique_ptr<RPCClient> createRPCClient(const Instance&) = 0;
};

// Factory
class CoreAdapterFactory {
public:
    static std::unique_ptr<CoreAdapter> create(CoreType type);
    static QList<CoreType> availableCores();  // Detect installed binaries
};
```

### RPC Multi-Instance Design

```
RPCClientPool (singleton)
├── Map<InstanceId, RPCClient> activeConnections
├── getClient(instanceId) → RPCClient*
├── removeClient(instanceId)
└── broadcast(method, args) → Map<InstanceId, Result>
```

Each running Instance gets:
- Unique socket name: `throne-instance-{instanceId}`
- Independent `CoreProcess` with its own `QLocalSocket`
- Independent `API::Client` in the pool

---

## 3. Component Interaction Diagrams

### Starting an Instance

```
UI: User clicks "Start" on Instance "US-East"
    ↓
InstanceManager::startInstance("US-East")
    ↓
PortAllocator::verifyAndBind(instance) → OK
    ↓
XrayAdapter::generateConfig(instance, profile)
    ↓
XrayAdapter::start(instance, config)
    ├── CoreProcess::Start() with env: THRONE_CORE_SOCKET=throne-instance-US-East
    ├── Wait for RPC connect (timeout 10s)
    ├── RPCClientPool::register(instanceId, client)
    └── HealthMonitor::onInstanceStarted(instanceId)
    ↓
UI: Instance status → "Running", show PID, ports, logs
```

### Chain with Core Selection

```
Profile: Chain [VMess → Xray VLESS → Trojan]
    ↓
Chain.outbound.core_type = "xray" (user selected)
    ↓
ConfigGenerator::buildChain(chain)
    ├── Detects Xray Core required
    ├── Generates Xray JSON config for full chain
    └── Returns Config{isXray: true, xrayConfig: "..."}
    ↓
XrayAdapter::start(instance, config)
    ├── Starts Xray process (not sing-box)
    └── RPC via Xray API
```

---

## 4. Configuration Flow

### Per-Instance Config Generation

```
Input: Instance + Profile (+ Group + Routes + DNS)
    ↓
ConfigGenerator::generate(instance, profile)
    ├── Resolve Instance.core_type
    ├── Select CoreAdapter
    ├── Build base config (inbounds: socks://bind:socks_port, http://bind:http_port)
    ├── Build outbound chain from Profile
    │   ├── If Chain: resolve each hop's Core requirement
    │   │   ├── All same Core → single Core config
    │   │   └── Mixed Core → bridge via local SOCKS (CoreAdapter cooperation)
    │   └── If Custom Xray → force XrayAdapter
    ├── Apply Instance.core_config overrides
    ├── Apply DNS settings (Instance + Global)
    └── Return Config{coreType, singboxJSON, xrayJSON}
```

---

## 5. Migration Strategy

### Throne → CoreDeck (Single → Multi-Instance)

1. **On first CoreDeck run:**
   - Detect Throne database (`throne.db`) without `instances` table
   - Create Default Instance with UUID, name="Default"
   - Migrate all existing Groups → Default Instance
   - Migrate Settings → InstanceSettings (Default Instance) + GlobalSettings
   - Update `Groups.instance_id` = Default Instance UUID
   - Mark migration complete in DB

2. **Backward Compatibility:**
   - Throne can still open migrated DB (ignores new tables)
   - CoreDeck reads Throne settings as fallback for Default Instance

---

## 6. Build System Changes

### CMake Additions
```cmake
# New targets
add_library(core_adapters STATIC
    src/core_adapters/CoreAdapter.cpp
    src/core_adapters/XrayAdapter.cpp
    src/core_adapters/SingBoxAdapter.cpp
    src/core_adapters/ExternalCoreAdapter.cpp
)

add_library(instance_manager STATIC
    src/instance_manager/InstanceRegistry.cpp
    src/instance_manager/ProcessSupervisor.cpp
    src/instance_manager/PortAllocator.cpp
    src/instance_manager/HealthMonitor.cpp
    src/instance_manager/StateStore.cpp
)

target_link_libraries(instance_manager PRIVATE core_adapters database configs)
target_link_libraries(Throne PRIVATE instance_manager core_adapters)
```

---

## 7. Threading Model

| Thread | Responsibility |
|--------|----------------|
| Main (GUI) | Qt event loop, UI, user interactions |
| `DS_cores` (QThread) | Core process management (existing) |
| `LogThread` (QThread) | Log processing (existing) |
| `InstanceManager` | Single-threaded, called from Main |
| `ProcessSupervisor` | Uses `DS_cores` thread for QProcess |
| `RPCClientPool` | Each client has own I/O thread (existing pattern) |
| `HealthMonitor` | Timer-based checks on Main thread |

---

## 8. Configuration Files

| File | Purpose |
|------|---------|
| `throne.db` | SQLite database (all persistent state) |
| `config/` | Core binaries, geoip.dat, geosite.dat, certificates |
| `translations/` | Qt .qm files |
| `log/` | Application logs (rotated) |

---

## 9. External Interfaces

| Interface | Protocol | Consumers |
|-----------|----------|-----------|
| Local SOCKS/HTTP | socks5, HTTP/1.1 | Browsers, CLI tools, other apps |
| RPC (Internal) | Protobuf over Unix/Windows socket | CoreDeck UI only |
| Remote API (Optional) | HTTP/JSON | External tools (if enabled) |
| Deeplink | `throne://` | Browser, other apps |

---

## 10. Platform-Specific Notes

### Windows
- Wintun for TUN (future)
- WinCommander for elevation
- Firewall rules for inbound ports (if not loopback)
- WER crash dumps

### Linux
- systemd / polkit for privilege escalation
- AppArmor / SELinux considerations
- Desktop entry, MIME types
- TUN via `tun` device + `CAP_NET_ADMIN`

### macOS
- NetworkExtension for TUN (future)
- Code signing / notarization for distribution
- Quarantine attribute handling
- Launch at login via SMAppService

---

## 11. Known Architecture Decisions

| Decision | Status | Reference |
|----------|--------|-----------|
| Single GUI process, multi-Core processes | CONFIRMED | DECISIONS.md |
| UUID as Instance primary key | CONFIRMED | DECISIONS.md |
| Core Adapter pattern for extensibility | CONFIRMED | DECISIONS.md |
| PortAllocator central service | CONFIRMED | DECISIONS.md |
| Local Proxy Mode first, Central Router later | CONFIRMED | ROADMAP.md |
| Throne DB migration on first run | CONFIRMED | DECISIONS.md |
| Xray as default Core (user preference) | CONFIRMED | PROJECT_SPEC.md |
| No dynamic config switching on single Core | CONFIRMED | PROJECT_SPEC.md |

---

## 12. Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 0.1 | 2026-10-10 | Initial | Baseline Throne analysis + target architecture |