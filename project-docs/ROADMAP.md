# CoreDeck Development Roadmap

**Version:** 0.1 (Draft)  
**Status:** PLANNED  
**Last Updated:** 2026-10-10

---

## Phase Overview

| Phase | Name | Status | Target | Dependencies |
|-------|------|--------|--------|--------------|
| 0 | Repository Analysis & Baseline | IN_PROGRESS | Week 1 | — |
| 1 | Instance Data Model & Storage | TODO | Week 2 | Phase 0 |
| 2 | Default Instance & Multi-Instance Management | TODO | Week 3 | Phase 1 |
| 3 | Multiple Independent Core Processes | TODO | Week 4 | Phase 2 |
| 4 | Independent Ports & Instance Dashboard | TODO | Week 5 | Phase 3 |
| 5 | Independent Subscriptions & Profiles | TODO | Week 6 | Phase 2 |
| 6 | Startup, Auto-Start & State Recovery | TODO | Week 7 | Phase 3, 5 |
| 7 | Core Adapters (sing-box, External) | TODO | Week 8-9 | Phase 3 |
| 8 | Core Selection for Chain & Profiles | TODO | Week 10 | Phase 3, 7 |
| 9 | Central Router | TODO | Week 11-12 | Phase 4 |
| 10 | TUN & Cross-Platform Integration | TODO | Week 13-14 | Phase 4 |
| 11 | Testing, Optimization & Packaging | TODO | Week 15-16 | All |

---

## Phase 0: Repository Analysis & Baseline (Week 1)

### Objectives
- [x] Clone and analyze Throne repository structure
- [x] Understand build system, dependencies, architecture
- [x] Verify baseline build works
- [x] Create project documentation structure
- [x] Document current architecture (ARCHITECTURE.md)
- [x] Write project specification (PROJECT_SPEC.md)

### Deliverables
- [x] `project-docs/README.md` - Project overview
- [x] `project-docs/PROJECT_SPEC.md` - Complete requirements
- [x] `project-docs/ARCHITECTURE.md` - Current + target architecture
- [x] `project-docs/ROADMAP.md` - This file
- [x] `project-docs/AI_WORKFLOW.md` - Development rules
- [x] `project-docs/DEPENDENCIES_AND_LICENSES.md` - License compliance

### Exit Criteria
- [ ] Throne builds successfully from source
- [ ] All documentation files created and committed
- [ ] Baseline test run documented

---

## Phase 1: Instance Data Model & Storage (Week 2)

### Objectives
- Design Instance entity with all required fields
- Extend SQLite schema with `instances`, `instance_settings`, `port_allocations` tables
- Add `instance_id` foreign key to `groups` table
- Implement migration from Throne single-instance schema
- Create InstanceRepository (CRUD operations)

### Tasks
| ID | Task | Status | Acceptance |
|----|------|--------|------------|
| MI-001 | Design Instance C++ class with all fields | TODO | Class compiles, serializes to/from JSON |
| MI-002 | Add Instance tables to Database.cpp | TODO | Migration runs on first launch |
| MI-003 | Implement InstanceRepository | TODO | CRUD operations work, unit tests pass |
| MI-004 | Add instance_id to Group entity & GroupsRepo | TODO | Groups queryable by instance_id |
| MI-005 | Write migration logic (Throne → CoreDeck) | TODO | Existing data preserved in Default Instance |
| MI-006 | Add InstanceSettings table & repository | TODO | Per-instance settings persist |

### Technical Details
- UUID generation: `QUuid::createUuid()`
- Migration: Detect missing `instances` table → create Default Instance → move all Groups → update settings
- Backward compatibility: Throne can still read DB (ignores new tables)

### Exit Criteria
- [ ] Database migration tested with real Throne data
- [ ] Instance CRUD operations verified
- [ ] No data loss in migration

---

## Phase 2: Default Instance & Multi-Instance Management (Week 3)

### Objectives
- Create Default Instance on first run
- Implement InstanceManager (registry, lifecycle)
- Add Instance selector to UI (toolbar/sidebar)
- Instance create/edit/delete dialogs
- Persist Instance list and selection

### Tasks
| ID | Task | Status | Acceptance |
|----|------|--------|------------|
| MI-010 | Implement InstanceManager singleton | TODO | Manages Instance lifecycle |
| MI-011 | Default Instance creation on first run | TODO | Default Instance exists with valid ports |
| MI-012 | Instance selector UI component | TODO | Switch between Instances in UI |
| MI-013 | Instance create/edit/delete dialogs | TODO | All fields editable, validation works |
| MI-014 | Instance list persistence | TODO | Survives app restart |
| MI-015 | Instance rename preserves data | TODO | UUID unchanged, all relations intact |

### UI Changes
- Toolbar: Instance dropdown (shows name, status indicator)
- Sidebar: Instance list with enable/disable toggle
- Settings: Instance management page

### Exit Criteria
- [ ] Can create 5+ Instances
- [ ] Switching Instances updates profile/group views
- [ ] All Instance data persists across restarts
- [ ] Deleting Instance cleans up only its data

---

## Phase 3: Multiple Independent Core Processes (Week 4)

### Objectives
- Refactor CoreProcess to support multiple simultaneous processes
- Implement ProcessSupervisor (per-Instance process management)
- Unique RPC socket per Instance
- RPC client pool for multi-connection management
- Health monitoring per Instance

### Tasks
| ID | Task | Status | Acceptance |
|----|------|--------|------------|
| MI-020 | Refactor CoreProcess for multi-instance | TODO | Multiple CoreProcess objects can run |
| MI-021 | Unique RPC socket naming per Instance | TODO | `throne-instance-{uuid}` format |
| MI-022 | Implement ProcessSupervisor | TODO | Start/stop/restart per Instance |
| MI-023 | RPCClientPool for connection management | TODO | Routes calls to correct Instance |
| MI-024 | HealthMonitor with crash detection | TODO | Tracks PID, RPC status, auto-restart |
| MI-025 | Run 2 Xray Instances simultaneously | TODO | Both running, independent configs |

### Key Changes
- `CoreProcess` constructor takes `instanceId` and `socketName`
- `API::Client` created per running Instance
- `THRONE_CORE_SOCKET` env var includes Instance ID
- ProcessSupervisor owns CoreProcess lifecycle

### Exit Criteria
- [ ] Two Xray processes run simultaneously
- [ ] Each has unique PID, ports, RPC socket
- [ ] Stopping one doesn't affect the other
- [ ] Core crash in one doesn't crash the other
- [ ] Logs show separate streams per Instance

---

## Phase 4: Independent Ports & Instance Dashboard (Week 5)

### Objectives
- PortAllocator for central port management
- Per-Instance SOCKS/HTTP/API ports
- Port conflict detection (UI + runtime)
- Instance dashboard showing status, ports, logs

### Tasks
| ID | Task | Status | Acceptance |
|----|------|--------|------------|
| MI-030 | Implement PortAllocator service | TODO | Allocates, tracks, releases ports |
| MI-031 | Port conflict detection in UI | TODO | Red warning on duplicate port entry |
| MI-032 | Runtime port bind verification | TODO | Fails gracefully if port taken |
| MI-033 | Instance dashboard UI | TODO | Shows status, PID, ports, traffic |
| MI-034 | Per-Instance log filtering | TODO | Log viewer filters by Instance |
| MI-035 | Port change applies without restart | TODO | Running Instance updates config |

### Port Allocation Strategy
- Default base: SOCKS 10808, HTTP 10809, API 10810
- Increment by 10 per Instance (10808, 10818, 10828...)
- Always verify at bind time (race condition guard)
- User can override any port

### Exit Criteria
- [ ] 5 Instances each with unique ports
- [ ] Port conflict detected in UI before save
- [ ] Runtime bind failure handled gracefully
- [ ] Dashboard shows real-time status per Instance
- [ ] Logs filterable by Instance

---

## Phase 5: Independent Subscriptions & Profiles (Week 6)

### Objectives
- Each Instance has independent Group list
- Subscription fetch/update/delete per Instance
- Profile operations scoped to Instance
- Same URL in multiple Instances → independent data

### Tasks
| ID | Task | Status | Acceptance |
|----|------|--------|------------|
| MI-040 | Group UI scoped to current Instance | TODO | Groups only show for active Instance |
| MI-041 | Subscription fetch per Instance | TODO | Instance A fetch doesn't affect B |
| MI-042 | Profile operations per Instance | TODO | Add/remove/test profiles per Instance |
| MI-043 | Same URL in two Instances test | TODO | Independent profile sets created |
| MI-044 | Delete Instance cascades Groups/Profiles | TODO | Other Instances unaffected |

### Exit Criteria
- [ ] Instance A and B have different Group lists
- [ ] Updating subscription in A doesn't change B
- [ ] Deleting Instance A removes its Groups only
- [ ] Profile latency tests work per Instance

---

## Phase 6: Startup, Auto-Start & State Recovery (Week 7)

### Objectives
- Per-Instance auto-start configuration
- Last profile restoration per Instance
- Crash recovery with rate limiting
- Clean exit vs crash distinction

### Tasks
| ID | Task | Status | Acceptance |
|----|------|--------|------------|
| MI-050 | Auto-start setting per Instance | TODO | Enabled Instances start on launch |
| MI-051 | Staggered startup (startup_delay_ms) | TODO | Instances start with delays |
| MI-052 | Restore last active profile | TODO | Correct profile selected per Instance |
| MI-053 | Crash detection & auto-restart | TODO | Restarts with 10s rate limit |
| MI-054 | Clean exit handling | TODO | No auto-restart on clean quit |
| MI-055 | No duplicate processes on restart | TODO | PID/RPC check prevents duplicates |

### Exit Criteria
- [ ] 3 Instances auto-start sequentially
- [ ] Each restores its last profile
- [ ] Crash restart works with rate limit
- [ ] App restart doesn't create duplicate processes

---

## Phase 7: Core Adapters (sing-box, External) (Week 8-9)

### Objectives
- Implement CoreAdapter abstraction
- XrayAdapter (refactor existing Xray logic)
- SingBoxAdapter (refactor existing sing-box logic)
- ExternalCoreAdapter (generic binary support)
- Core detection and capability reporting

### Tasks
| ID | Task | Status | Acceptance |
|----|------|--------|------------|
| MI-060 | CoreAdapter abstract base class | TODO | Interface defined, compilable |
| MI-061 | XrayAdapter implementation | TODO | Generates Xray config, starts process |
| MI-062 | SingBoxAdapter implementation | TODO | Generates sing-box config, starts process |
| MI-063 | ExternalCoreAdapter implementation | TODO | Starts user binary, bridges via SOCKS |
| MI-064 | CoreAdapterFactory with detection | TODO | Detects available cores on system |
| MI-065 | Core capability matrix | TODO | Protocol/Core compatibility table |

### Exit Criteria
- [ ] Can select Core type per Instance
- [ ] Xray Instance runs Xray process
- [ ] sing-box Instance runs sing-box process
- [ ] External Core runs user binary
- [ ] Capability detection works

---

## Phase 8: Core Selection for Chain & Profiles (Week 10)

### Objectives
- Chain-level Core selection (Xray vs sing-box)
- Profile-level Core selection for Custom Xray
- Validation of Core/protocol compatibility
- Mixed-Core Chain bridging via local SOCKS

### Tasks
| ID | Task | Status | Acceptance |
|----|------|--------|------------|
| MI-070 | Add core_type to Chain outbound | TODO | UI shows Core selector for Chain |
| MI-071 | Custom Xray Outbound forces Xray | TODO | Xray config generated for Custom |
| MI-072 | ConfigGenerator routes to correct Adapter | TODO | Chain with Xray Core uses XrayAdapter |
| MI-073 | Mixed Core Chain bridging | TODO | sing-box ↔ Xray via local SOCKS |
| MI-074 | Validation: reject incompatible combos | TODO | Error shown for unsupported protocol |

### Reference
- v2rayN Chain Core selection behavior
- Throne's existing Custom Xray FullConfig bridge

### Exit Criteria
- [ ] Chain with Xray Core runs on Xray
- [ ] Chain with sing-box Core runs on sing-box
- [ ] Mixed Chain bridges correctly
- [ ] Invalid combinations rejected with clear error

---

## Phase 9: Central Router (Week 11-12)

### Objectives
- Central entry port (e.g., 10800)
- Rule-based routing to Instance ports
- Rule types: domain, IP/CIDR, manual
- Health-aware routing (skip down Instances)
- Fallback/default Instance

### Tasks
| ID | Task | Status | Acceptance |
|----|------|--------|------------|
| MI-080 | CentralRouter configuration model | TODO | Rules definable in UI |
| MI-081 | sing-box as router implementation | TODO | Router config generates sing-box JSON |
| MI-082 | Rule evaluation engine | TODO | Domain/IP matching works |
| MI-083 | Health-aware routing | TODO | Down Instances skipped |
| MI-084 | Router UI (rules, priorities, fallback) | TODO | Visual rule editor |
| MI-085 | Router enable/disable per Instance | TODO | Independent ports still work |

### Exit Criteria
- [ ] Central router port accepts connections
- [ ] Rules route to correct Instance port
- [ ] Down Instance skipped automatically
- [ ] Independent ports still accessible
- [ ] No routing loops possible

---

## Phase 10: TUN & Cross-Platform Integration (Week 13-14)

### Objectives
- Per-Instance TUN interface
- Platform-specific TUN implementations
- System Proxy ownership (single active Instance)
- Kill switch per Instance

### Tasks
| ID | Task | Status | Acceptance |
|----|------|--------|------------|
| MI-090 | TUN interface per Instance (Windows) | TODO | Wintun adapter per Instance |
| MI-091 | TUN interface per Instance (Linux) | TODO | tun device per Instance |
| MI-092 | TUN interface per Instance (macOS) | TODO | NetworkExtension per Instance |
| MI-093 | System Proxy single-owner model | TODO | Only one Instance controls SP |
| MI-094 | Kill switch per Instance | TODO | Blocks traffic if Instance down |
| MI-095 | TUN/SP integration testing | TODO | All platforms verified |

### Exit Criteria
- [ ] TUN works on all 3 platforms
- [ ] System Proxy transfers cleanly between Instances
- [ ] Kill switch activates on Instance crash
- [ ] No DNS leaks in TUN mode

---

## Phase 11: Testing, Optimization & Packaging (Week 15-16)

### Objectives
- Regression testing of Throne features
- Performance benchmarks (memory, CPU)
- Multi-platform build verification
- Release packaging (AppImage, .exe, .dmg, .deb)
- Documentation completion

### Tasks
| ID | Task | Status | Acceptance |
|----|------|--------|------------|
| MI-100 | Throne feature regression suite | TODO | All existing features work |
| MI-101 | Memory benchmark (5 Instances) | TODO | < 200 MB total |
| MI-102 | CPU benchmark (5 Instances idle) | TODO | < 2% total |
| MI-103 | Startup time benchmark | TODO | < 10s for 5 auto-start |
| MI-104 | Windows build & installer | TODO | .exe + portable ZIP |
| MI-105 | Linux build (AppImage, deb) | TODO | Packages install & run |
| MI-106 | macOS build & notarization | TODO | .app runs without quarantine |
| MI-107 | Complete all documentation | TODO | All docs current |

### Exit Criteria
- [ ] All acceptance criteria from PROJECT_SPEC.md met
- [ ] Performance targets achieved
- [ ] Packages built for all platforms
- [ ] Documentation complete and accurate

---

## Parallelization Opportunities

| Phases | Can Run Parallel | Reason |
|--------|------------------|--------|
| 1 + 2 | Partial | DB schema needed before UI |
| 3 + 5 | Yes | Core processes independent of subscription UI |
| 7 + 8 | Yes | Adapters needed before Chain selection |
| 9 + 10 | No | Router needs stable Local Proxy Mode first |

---

## Risk Register

| Risk | Impact | Likelihood | Mitigation |
|------|--------|------------|------------|
| Port conflicts at scale | High | Medium | PortAllocator with bind-time verification |
| Core process resource exhaustion | High | Low | HealthMonitor with limits, user config |
| Migration data loss | Critical | Low | Automated tests with real Throne DBs |
| Cross-platform TUN complexity | High | High | Defer to Phase 10, use sing-box abstraction |
| RPC socket cleanup on crash | Medium | Medium | RAII + health monitor cleanup |

---

## Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 0.1 | 2026-10-10 | Initial | Draft roadmap from master prompt |