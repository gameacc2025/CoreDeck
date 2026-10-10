# CoreDeck Architectural Decisions

**Version:** 0.1  
**Last Updated:** 2026-10-10

---

## Decision Record Format

| Field | Description |
|-------|-------------|
| **ID** | Unique identifier (DEC-XXX) |
| **Date** | Decision date |
| **Status** | PROPOSED / ACCEPTED / SUPERSEDED / REJECTED |
| **Context** | Problem or situation requiring decision |
| **Options** | Alternatives considered |
| **Decision** | Chosen option |
| **Rationale** | Why this option was chosen |
| **Consequences** | Trade-offs, follow-up work needed |

---

## DEC-001: Single GUI Process, Multiple Core Processes

**Date:** 2026-10-10  
**Status:** ACCEPTED

### Context
Throne currently runs a single Core process (sing-box with optional Xray bridge). Users need multiple independent proxy environments (like running multiple v2rayN instances).

### Options
1. **Single Core with dynamic config switching** - One Core process, swap config on profile change
2. **Multiple Core processes, single GUI** - Each Instance gets its own Core process
3. **Multiple GUI processes** - Fork Throne, run separate apps (current user workaround)

### Decision
Option 2: **Single GUI process managing multiple independent Core processes**

### Rationale
- Resource efficiency: One Qt event loop, one UI, shared libraries
- True isolation: Crash in one Core doesn't affect others
- Independent ports, configs, subscriptions per Instance
- Matches user mental model: "One app, many proxy environments"
- v2rayN reference uses separate processes per instance

### Consequences
- Need ProcessSupervisor to manage multiple CoreProcess objects
- Unique RPC socket per Instance (`throne-instance-{uuid}`)
- RPCClientPool to route calls to correct Core
- PortAllocator for port management
- HealthMonitor per Instance

---

## DEC-002: UUID as Instance Primary Key

**Date:** 2026-10-10  
**Status:** ACCEPTED

### Context
Instances need stable identifiers for database relationships, RPC socket names, and configuration references.

### Options
1. **Auto-increment integer ID** - Simple, but leaks creation order
2. **UUID (QUuid)** - Globally unique, no coordination needed
3. **User-provided string ID** - Human readable, but collision risk

### Decision
Option 2: **UUID v4 (random) as primary key**

### Rationale
- No central ID allocation needed
- Stable across renames, migrations, exports
- Can be generated client-side
- Standard practice for distributed entities
- Used in Throne for other entities (e.g., scan sessions)

### Consequences
- All Instance-related tables use TEXT UUID primary keys
- Foreign keys: `groups.instance_id`, `instance_settings.instance_id`, `port_allocations.instance_id`
- RPC socket names include UUID: `throne-instance-{uuid}`
- UI displays name, uses UUID internally

---

## DEC-003: Core Adapter Pattern for Extensibility

**Date:** 2026-10-10  
**Status:** ACCEPTED

### Context
Throne currently hardcodes sing-box as primary with Xray bridge for Xray-specific configs. CoreDeck needs: Xray as primary, sing-box as secondary, external cores in future.

### Options
1. **Monolithic config generator** - Big switch statements for each Core type
2. **Strategy Pattern (CoreAdapter)** - Abstract interface per Core type
3. **Plugin system** - Dynamic loading of Core adapters

### Decision
Option 2: **CoreAdapter abstract base class with concrete implementations**

### Rationale
- Clean separation: Core-specific logic encapsulated
- Easy to add new Core types (ExternalCoreAdapter)
- Compile-time polymorphism, no dynamic loading complexity
- Testable: mock adapters for unit tests
- Follows existing Throne pattern (outbound classes)

### Consequences
- New files: `CoreAdapter.h`, `XrayAdapter.cpp`, `SingBoxAdapter.cpp`, `ExternalCoreAdapter.cpp`
- Factory: `CoreAdapterFactory::create(CoreType)`
- ConfigGenerator routes to `adapter->generateConfig()`
- ProcessSupervisor uses `adapter->start()/stop()`
- Capability matrix: `adapter->supportsProtocol()`

---

## DEC-004: Central PortAllocator Service

**Date:** 2026-10-10  
**Status:** ACCEPTED

### Context
Multiple Instances need unique SOCKS/HTTP/API ports. Port conflicts must be prevented at config time and detected at runtime.

### Options
1. **Decentralized** - Each Instance picks ports, check on start
2. **Central PortAllocator** - Single service tracks all allocations
3. **OS-assigned (port 0)** - Let OS pick, read back actual port

### Decision
Option 2: **Central PortAllocator with bind-time verification**

### Rationale
- Prevents conflicts before user saves config
- Tracks all ports in database (`port_allocations` table)
- Suggests next available port automatically
- Bind-time check catches race conditions (port taken between check and bind)
- User can still override manually

### Consequences
- PortAllocator reserves ports on Instance create/edit
- ProcessSupervisor verifies bind at start
- Port changes update database and restart Core if running
- Default base ports: SOCKS 10808, HTTP 10809, API 10810 (+10 per Instance)

---

## DEC-005: Local Proxy Mode First, Central Router Later

**Date:** 2026-10-10  
**Status:** ACCEPTED

### Context
Users want to connect different browsers/tools to different Instances. Long-term goal is central router for rule-based routing.

### Options
1. **Build Central Router first** - Single entry port, rule engine routes to Instances
2. **Local Proxy Mode first** - Each Instance exposes own ports, apps connect directly
3. **Both simultaneously** - Complex, high risk

### Decision
Option 2: **Local Proxy Mode as MVP (Phases 1-5), Central Router in Phase 9+**

### Rationale
- Simpler: No routing logic, no rule engine, no DNS leak concerns
- Immediate value: Users can use Instance ports directly today
- Browser profiles / proxy switchers can target specific Instance ports
- Central Router is complex: rule conflicts, health checks, loop prevention
- Deferring reduces Phase 1-5 scope and risk
- sing-box can serve as router later (native routing support)

### Consequences
- Phase 1-5: Each Instance has independent SOCKS/HTTP ports
- Phase 9+: CentralRouter component added, optional
- Independent ports remain functional even if router fails
- Router UI separate from Instance dashboard

---

## DEC-006: Throne Database Migration on First Run

**Date:** 2026-10-10  
**Status:** ACCEPTED

### Context
CoreDeck extends Throne's schema. Existing Throne users must migrate seamlessly.

### Options
1. **Separate database file** - `coredeck.db` alongside `throne.db`
2. **In-place migration** - Add tables/columns to existing `throne.db`
3. **New database, import wizard** - User manually imports

### Decision
Option 2: **In-place migration with backward compatibility**

### Rationale
- Zero friction for existing users
- Throne can still read migrated DB (ignores new tables/columns)
- Single source of truth
- Atomic migration on first CoreDeck launch
- Default Instance created with all existing data

### Consequences
- Migration code in `Database.cpp` or new `Migration.cpp`
- Detect: `instances` table missing → run migration
- Create Default Instance UUID
- Move all Groups → Default Instance
- Copy Settings → InstanceSettings (Default Instance) + Global Settings
- Add `instance_id` column to `groups` table
- Mark migration complete (version table or flag)

---

## DEC-007: Xray as Default Core (User Preference)

**Date:** 2026-10-10  
**Status:** ACCEPTED

### Context
Master prompt specifies Xray as primary Core. Throne uses sing-box as primary.

### Options
1. **Keep sing-box default** - Minimal change, but contradicts user requirement
2. **Xray default** - Aligns with user requirement, Xray has wider protocol support
3. **User chooses on first run** - Adds friction

### Decision
Option 2: **Xray as default Core for new Instances**

### Rationale
- Explicit user requirement in master prompt
- Xray supports more protocols natively (VLESS XTLS, Reality, etc.)
- sing-box still available as alternative
- Throne already has Xray integration (Custom Xray Outbound/FullConfig)
- CoreAdapter pattern makes both equal citizens

### Consequences
- Default Instance: `core_type = "xray"`
- XrayAdapter is primary implementation
- sing-box Adapter for protocols Xray doesn't support
- Core detection prefers Xray binary
- Documentation reflects Xray-first

---

## DEC-008: No Dynamic Config Switching on Single Core

**Date:** 2026-10-10  
**Status:** ACCEPTED

### Context
Alternative to multi-Core: run one Core, dynamically reload config when user switches profile/Instance.

### Decision
**Explicitly rejected.** Each Instance runs its own Core process.

### Rationale
- Violates isolation requirement (crash affects all)
- Config reload not atomic; connection disruption
- Port binding conflicts (single Core can't bind multiple SOCKS ports)
- State mixing: traffic stats, connections, DNS cache
- Complexity: dynamic config API vs. process restart
- User expectation: "independent environments" = independent processes

### Consequences
- Confirmed: ProcessSupervisor manages multiple CoreProcess objects
- Resource usage higher but acceptable (sing-box/Xray ~10-20MB each)
- HealthMonitor tracks each independently

---

## DEC-009: Instance Settings Separate from Global Settings

**Date:** 2026-10-10  
**Status:** ACCEPTED

### Context
Throne has single `settings` table (key-value). CoreDeck needs per-Instance settings (ports, auto-start, core_type) and global settings (UI theme, language).

### Options
1. **Extend settings table with instance_id column** - NULL = global
2. **Separate instance_settings table** - Clear separation
3. **JSON blob in instances table** - Simple but less queryable

### Decision
Option 2: **Separate `instance_settings` table + keep global `settings` table**

### Rationale
- Clear semantic separation: Instance vs Global
- Foreign key with CASCADE DELETE on Instance removal
- Easy to query: `SELECT * FROM instance_settings WHERE instance_id = ?`
- Global settings unchanged for backward compatibility
- Migration copies relevant settings to InstanceSettings

### Consequences
- New table: `instance_settings (instance_id, key, value, PK(instance_id, key))`
- SettingsRepo extended or new InstanceSettingsRepo
- UI: Global settings in main Settings dialog, Instance settings in Instance dialog

---

## DEC-010: Chain Core Selection Stored in Chain Config

**Date:** 2026-10-10  
**Status:** ACCEPTED

### Context
v2rayN allows selecting Core per Chain. Throne's Chain outbound has no Core field.

### Decision
**Add `core_type` field to Chain outbound config, persisted in profile JSON**

### Rationale
- Chain-level Core selection is explicit user choice
- Should not depend on Instance default Core
- Changing Instance default Core shouldn't change Chain behavior
- Stored in Profile.outbound JSON (chain.list + chain.core_type)
- ConfigGenerator reads chain.core_type and routes to correct Adapter

### Consequences
- Chain.h: add `core_type` field (default: Instance default)
- Chain UI: Core selector dropdown
- generate.cpp: inspect chain.core_type, delegate to Adapter
- Mixed-Core Chain: bridge via local SOCKS (future work)

---

## Decision Index

| ID | Title | Status |
|----|-------|--------|
| DEC-001 | Single GUI, Multiple Core Processes | ACCEPTED |
| DEC-002 | UUID as Instance Primary Key | ACCEPTED |
| DEC-003 | Core Adapter Pattern | ACCEPTED |
| DEC-004 | Central PortAllocator | ACCEPTED |
| DEC-005 | Local Proxy Mode First | ACCEPTED |
| DEC-006 | In-Place DB Migration | ACCEPTED |
| DEC-007 | Xray as Default Core | ACCEPTED |
| DEC-008 | No Dynamic Config Switching | ACCEPTED (Rejected Alternative) |
| DEC-009 | Separate Instance Settings Table | ACCEPTED |
| DEC-010 | Chain Core Selection in Chain Config | ACCEPTED |

---

*Decisions are final once ACCEPTED. Use SUPERSEDED for changes. Document all significant architectural choices here.*