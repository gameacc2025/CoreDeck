# CoreDeck Project Specification

**Version:** 0.2 (Draft)  
**Status:** PLANNED  
**Last Updated:** 2026-10-10

---

## 1. Product Vision

CoreDeck is a **multi-instance proxy client** derived from [Throne](https://github.com/throneproj/Throne). The required Throne code lives inside CoreDeck's own repository and CoreDeck is developed independently (DEC-011). It enables a single UI to manage multiple independent proxy environments ("Instances") simultaneously, replacing the need to run multiple separate GUI applications (like multiple v2rayN instances) for different proxy configurations.

### Core Value Proposition
- **One UI, Many Instances** — Manage 5-10+ independent proxy environments from a single window
- **Resource Efficiency** — Single Qt application process vs. multiple GUI processes
- **Isolation** — Each Instance has independent Core, ports, subscriptions, profiles, and state
- **Extensibility** — Plugin architecture for Core adapters (Xray, sing-box, external)

---

## 2. Terminology

| Term | Definition |
|------|------------|
| **Instance** | An independent proxy environment with its own Core process, ports, subscriptions, profiles, and settings |
| **Core** | The underlying proxy engine (Xray-core, sing-box, or external binary) |
| **Profile** | A single proxy server configuration (VMess, VLESS, Trojan, Shadowsocks, etc.) |
| **Group** | A subscription group containing multiple profiles fetched from a URL |
| **Chain** | A multi-hop configuration where traffic passes through multiple profiles sequentially |
| **Core Adapter** | Abstraction layer for managing different Core types uniformly |
| **Port Allocator** | Central service for assigning and tracking local proxy ports per Instance |

---

## 3. Instance Definition

Each Instance **must** have the following persistent attributes:

| Attribute | Type | Required | Description |
|-----------|------|----------|-------------|
| `id` | UUID | Yes | Immutable unique identifier |
| `name` | String | Yes | User-editable display name |
| `enabled` | Boolean | Yes | Whether Instance is active |
| `core_type` | Enum | Yes | `xray`, `singbox`, `external` |
| `core_path` | String | No | Path to Core binary (if external/custom) |
| `core_args` | String | No | Additional CLI arguments for Core |
| `core_config` | Object | No | Core-specific configuration overrides |
| `socks_port` | Integer | Yes | Local SOCKS proxy port |
| `http_port` | Integer | Yes | Local HTTP proxy port |
| `api_port` | Integer | No | Management API port (if applicable) |
| `bind_address` | String | Yes | Default: `127.0.0.1` |
| `auto_start` | Boolean | Yes | Start Core on application launch |
| `restart_on_crash` | Boolean | Yes | Default `false` (see section 8) |
| `restore_last_profile` | Boolean | Yes | Default `true` (see section 8) |
| `startup_delay_ms` | Integer | Yes | Default `0` (see section 8) |
| `last_profile_id` | Integer | No | Last selected profile ID |
| `last_active_profile_id` | Integer | No | Last successfully connected profile |
| `dns_settings` | Object | No | DNS/routing overrides for this Instance |
| `created_at` | Timestamp | Yes | Creation time |
| `updated_at` | Timestamp | Yes | Last modification time |

### Persistence Requirements
- All Instance data **must survive** application restart
- Data stored in SQLite database (extending the Throne-format `throne.db` in place; see DEC-006, DEC-015)
- Instance `id` (UUID) is the **primary key** for all relationships — never use display name

---

## 4. Default Instance Behavior

### On First Launch
1. Create a **Default Instance** with:
   - `id`: Generated UUID
   - `name`: "Default"
   - `core_type`: `xray`; if no Xray binary is found, fall back to `singbox` (decided in application logic, not in the DB default)
   - `core_path`: Auto-detected by CoreDeck's core management (inherited from Throne)
   - `socks_port`: 10808 (or next available)
   - `http_port`: 10809 (or next available)
   - `bind_address`: `127.0.0.1`
   - `auto_start`: `false`
   - `enabled`: `true`

2. User **must not** be required to configure anything to start using the Default Instance

### On Creating New Instance
- Pre-fill with sensible defaults (same as Default Instance but with incremented ports)
- Ports **must not conflict** with existing Instances or system-reserved ports
- `auto_start`: `false` by default
- User can modify all fields before saving

### Port Allocation Rules
- Suggested starting ports: SOCKS 10808, HTTP 10809, API 10810 (per Instance)
- **Always verify** port availability at bind time (race condition possible)
- If default port taken, auto-assign next available in range 10808-11000
- User can override any port manually
- Port changes **must** update running Core config if Instance is active

---

## 5. Subscription & Profile Independence

### Requirements
1. **Each Instance owns its Groups** — Groups have an `instance_id` foreign key
2. **Profiles belong to Groups** — Indirectly tied to Instance via Group
3. **No cross-Instance leakage** — Adding/updating/deleting a Group in Instance A never affects Instance B
4. **Same subscription URL in multiple Instances** — Each fetches and stores independently
5. **Renaming Instance** — Must not break any data associations (use UUID)
6. **Deleting Instance** — Must cascade delete its Groups, Profiles, Routes, but **never** touch other Instances

### Migration from Throne
- Existing Throne data (single global Group/Profile space) → migrate to Default Instance
- Preserve all existing profiles, subscriptions, settings
- Create Default Instance with current Throne data
- New Instances start empty

---

## 6. Core Management

### Supported Cores (Priority Order)
1. **Xray-core** — Primary target, full feature parity with Throne
2. **sing-box** — Secondary, for protocols Xray doesn't support
3. **External Core** — User-specified binary with SOCKS/HTTP output

### Core Adapter Interface
Each Core type implements:
```cpp
class CoreAdapter {
    virtual bool validateConfig(const Config&) = 0;
    virtual Config generateConfig(const Instance&, const Profile&) = 0;
    virtual ProcessStartResult start(const Instance&, const Config&) = 0;
    virtual void stop(const Instance&) = 0;
    virtual void restart(const Instance&) = 0;
    virtual InstanceStatus getStatus(const Instance&) = 0;
    virtual Logs getLogs(const Instance&) = 0;
    virtual bool supportsChain() = 0;
    virtual bool supportsProtocol(Protocol) = 0;
}
```

### Process Management Rules
- Each Instance runs **its own Core process** (separate PID)
- Stopping Instance A **never** stops Instance B
- Core crash in Instance A **never** affects Instance B
- Each Core process has:
  - Independent config file (generated per start)
  - Independent log stream
  - Independent RPC socket (unique name per Instance)
  - Independent working directory (optional)
- **No** single Core process with dynamic config switching to simulate multi-Instance

### Health Monitoring
- Track: PID, start time, CPU/memory, RPC connectivity, last error
- Auto-restart on crash (configurable per Instance, default: off)
- Max restart rate: 1 per 10 seconds (prevent loop)
- Log all state transitions

---

## 7. Port Management

### Port Types Per Instance
| Port | Protocol | Default | Configurable |
|------|----------|---------|--------------|
| SOCKS | socks5 | 10808 + N*10 | Yes |
| HTTP | http | 10809 + N*10 | Yes |
| API | gRPC/HTTP | 10810 + N*10 | Yes (if Core supports) |

### Port Allocator
- Central service tracking all allocated ports
- On Instance create: reserve ports, verify availability
- On Instance start: bind verify (race condition guard)
- On Instance delete: release ports
- On port change: update config, restart Core if running
- Bind address default: `127.0.0.1` (loopback only)

---

## 8. Auto-Start & State Recovery

### Per-Instance Settings
| Setting | Type | Default | Description |
|---------|------|---------|-------------|
| `auto_start` | Boolean | `false` | Start Core on app launch |
| `restart_on_crash` | Boolean | `false` | Auto-restart Core if it crashes |
| `restore_last_profile` | Boolean | `true` | Reconnect last active profile |
| `startup_delay_ms` | Integer | `0` | Staggered start delay |

### Startup Sequence
1. App starts → load all Instances from DB
2. For each Instance where `enabled && auto_start`:
   - Wait `startup_delay_ms`
   - Generate config for `last_active_profile_id` (or `last_profile_id`)
   - Start Core process
   - Verify RPC connection
   - Mark Instance as running
3. Non-auto-start Instances remain stopped until user action

### Crash Recovery
- Distinguish **clean exit** (user quit) vs **crash** (signal, exception, non-zero exit)
- On clean exit: save `last_active_profile_id`, do NOT auto-restart unless `auto_start=true`
- On crash: if `restart_on_crash=true`, restart after 2s delay (max 1/10s)
- Never create duplicate Core processes on app restart (check PID + RPC)

---

## 9. Core Selection for Chain & Profiles

### Background
v2rayN allows selecting Core (Xray vs sing-box) per Chain/profile. Throne currently uses sing-box as primary with Xray for Xray-specific configs (Custom Xray Outbound/FullConfig).

### Requirements
1. **Chain-level Core selection** — User can choose Core for each Chain
2. **Profile-level Core selection** — For Custom Xray configs, explicitly use Xray
3. **Validation** — Reject incompatible Core/protocol combinations
4. **Execution model** — 
   - Chain with Xray Core → Run Xray process for that Chain
   - Chain with sing-box Core → Run sing-box process
   - Mixed Chain → Bridge via local SOCKS (sing-box ↔ Xray)
5. **Persistence** — Core choice stored in Chain/profile config, not Instance default

### Implementation Approach
- Extend `chain` outbound with `core_type` field
- Extend `Custom` outbound with explicit Core requirement
- During config generation: route to appropriate Core Adapter
- Bridge Core processes via local loopback SOCKS ports

---

## 10. Routing Architecture

### Phase 1: Local Proxy Mode (MVP)
- Each Instance exposes independent SOCKS/HTTP ports
- Applications connect directly to Instance's port
- Example: Browser A → `127.0.0.1:10808` (Instance 1), Browser B → `127.0.0.1:10818` (Instance 2)
- **No central router** — simplest, most stable

### Phase 2: Central Router (Future)
- Single entry port (e.g., 10800) routes to Instance ports based on rules
- Rule types: domain, IP/CIDR, protocol, process, manual
- Fallback/default Instance
- Health-aware routing (skip down Instances)
- DNS leak prevention
- **sing-box as router** (preferred) or dedicated router process

### Safety Requirements
- No routing loops
- No incompatible rule combinations
- Destination reachability check before route
- Independent ports remain functional if router fails
- Explicit user consent for system-wide proxy/TUN

---

## 11. System Proxy & TUN

### System Proxy
- **Single system-wide setting** — Cannot have multiple Instances controlling it simultaneously
- Design: One "active" Instance owns System Proxy at a time
- Switching active Instance → gracefully transfer System Proxy
- Preserve Throne's existing System Proxy behavior
- Changes must be reversible on app quit/crash

### TUN
- **Deferred to Phase 10+** — Do not block Phase 1-5
- Platform differences: Windows (Wintun), Linux (tun/tap + root)
- When implemented: per-Instance TUN with explicit user selection
- No automatic TUN activation without user action

---

## 12. Cross-Platform Support

| Platform | Status | Notes |
|----------|--------|-------|
| Windows 10/11 | Primary | Full support, Wintun for TUN |
| Linux | Supported | AppImage / deb / rpm; TUN requires root/capabilities |
| macOS | **Out of scope** | Dropped by owner decision (DEC-012) |

### Build Requirements
- Qt 6.x (Widgets, Network, LinguistTools, DBus on Linux)
- CMake 3.20+
- C++20
- SQLite3 (bundled via SQLiteCpp)
- Protobuf (simple-protobuf for RPC)

---

## 13. Security & Privacy

### Data Handling
- **Never log** subscription URLs, tokens, passwords, or private keys
- **Never commit** sensitive data to repository
- Local database: SQLite with no encryption (user's disk encryption assumed)
- API secrets: Generated on first run, stored in DB

### Network
- Bind to `127.0.0.1` by default — **no** `0.0.0.0` without explicit user action
- Validate all user-provided paths, arguments, URLs
- No automatic updates of Core binaries without verification

### Licenses
- CoreDeck: GPL-3.0 (inherits from Throne)
- Xray-core: MPL-2.0 (verify against the official repository before shipping)
- sing-box: GPL-3.0
- Qt: LGPL-3.0 / Commercial
- SQLiteCpp: MIT
- simple-protobuf: MIT
- All third-party licenses tracked in `DEPENDENCIES_AND_LICENSES.md`

---

## 14. Acceptance Criteria (MVP - Phase 3)

### Instance Management
- [ ] Default Instance created on first run
- [ ] Create/Edit/Delete Instances via UI
- [ ] Instance settings persist across restarts
- [ ] Renaming Instance preserves all data
- [ ] Deleting Instance cleans up only its data

### Multi-Core Execution
- [ ] Two Xray Instances run simultaneously
- [ ] Each has independent config, ports, PID
- [ ] Stopping one doesn't affect the other
- [ ] Core crash in one doesn't crash the other
- [ ] Logs separable per Instance

### Ports
- [ ] Each Instance has unique SOCKS/HTTP ports
- [ ] Port conflict detection (UI + runtime)
- [ ] Port changes apply without app restart
- [ ] Ports persist across restarts

### Subscriptions
- [ ] Each Instance has independent Group list
- [ ] Subscribe/Update/Delete in Instance A doesn't affect Instance B
- [ ] Same URL in two Instances → independent profile sets

### Startup
- [ ] Auto-start per Instance works
- [ ] Last profile restored per Instance
- [ ] No duplicate processes on restart
- [ ] Crash restart respects rate limit

---

## 15. Out of Scope (MVP)

- Central Router (Phase 9+)
- TUN per Instance (Phase 10+)
- Web UI / Remote management
- Profile synchronization across Instances
- Advanced traffic shaping / QoS
- Plugin marketplace
- Mobile / Android / iOS
- macOS

---

## 16. Success Metrics

| Metric | Target |
|--------|--------|
| Memory (5 Instances idle) | < 200 MB total |
| CPU (5 Instances idle) | < 2% total |
| Startup time (5 auto-start) | < 10 seconds |
| Port bind success rate | 100% (with allocator) |
| Core crash isolation | 100% (other Instances unaffected) |
| Settings persistence | 100% survival across restarts |

---

## 17. Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 0.1 | 2026-10-10 | Initial | Draft specification from master prompt |
| 0.2 | 2026-10-10 | Review | Missing Instance fields added, macOS dropped, Throne absorbed, Xray licence corrected |