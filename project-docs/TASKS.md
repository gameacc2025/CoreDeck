# CoreDeck Task List

**Version:** 0.1 (Draft)  
**Status:** PLANNED  
**Last Updated:** 2026-10-10

---

## Task Status Legend

| Status | Meaning |
|--------|---------|
| `TODO` | Not started |
| `IN_PROGRESS` | Currently being worked on |
| `BLOCKED` | Waiting on dependency or decision |
| `DONE` | Implementation complete |
| `VERIFIED` | Tested and confirmed working |

---

## Phase 0: Repository Analysis & Baseline

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-000 | Clone Throne repository | DONE | High | — | Throne/ | Repository cloned locally | `git status` in Throne/ |
| MI-001 | Analyze Throne architecture | DONE | High | MI-000 | ARCHITECTURE.md | Architecture documented | Review ARCHITECTURE.md |
| MI-002 | Verify baseline build | BLOCKED | High | MI-000 | BUILD_AND_TEST.md | Throne builds successfully | CMake build passes |
| MI-003 | Create project documentation structure | DONE | High | — | project-docs/ | All doc files created | `ls project-docs/` |
| MI-004 | Write PROJECT_SPEC.md | DONE | High | MI-001 | PROJECT_SPEC.md | Complete requirements documented | Review spec |
| MI-005 | Write ARCHITECTURE.md | DONE | High | MI-001 | ARCHITECTURE.md | Current + target architecture documented | Review arch |
| MI-006 | Write ROADMAP.md | DONE | High | MI-004 | ROADMAP.md | Phases and tasks defined | Review roadmap |
| MI-007 | Write AI_WORKFLOW.md | DONE | High | — | AI_WORKFLOW.md | Development rules documented | Review workflow |
| MI-008 | Write DEPENDENCIES_AND_LICENSES.md | DONE | High | MI-000 | DEPENDENCIES_AND_LICENSES.md | All licenses documented | Review licenses |

---

## Phase 1: Instance Data Model & Storage

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-010 | Design Instance C++ class | IN_PROGRESS | High | MI-002 | include/database/entities/Instance.h, src/database/entities/Instance.cpp, tests/test_instance.cpp | Class with all fields, JSON serialization | Unit test |
| MI-011 | Add Instance tables to Database.cpp | TODO | High | MI-010 | src/database/Database.cpp, Schema.sql | Migration creates tables on first run | Migration test |
| MI-012 | Implement InstanceRepository | TODO | High | MI-011 | src/database/InstanceRepository.h, .cpp | CRUD operations work | Unit test |
| MI-013 | Add instance_id to Group entity | TODO | High | MI-011 | include/database/entities/Group.h, GroupsRepo.cpp | Groups queryable by instance_id | Integration test |
| MI-014 | Write migration logic (Throne → CoreDeck) | TODO | High | MI-011 | src/database/Migration.cpp | Existing data moves to Default Instance | Migration test with real DB |
| MI-015 | Add InstanceSettings table & repo | TODO | Medium | MI-011 | src/database/InstanceSettingsRepo.h, .cpp | Per-instance settings persist | Unit test |
| MI-016 | Add PortAllocations table & repo | TODO | Medium | MI-011 | src/database/PortAllocationRepo.h, .cpp | Port tracking works | Unit test |

---

## Phase 2: Default Instance & Multi-Instance Management

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-020 | Implement InstanceManager singleton | TODO | High | MI-012 | src/instance/InstanceManager.h, .cpp | Manages Instance lifecycle | Integration test |
| MI-021 | Default Instance creation on first run | TODO | High | MI-014, MI-020 | InstanceManager.cpp | Default Instance exists with valid ports | First-run test |
| MI-022 | Instance selector UI component | TODO | High | MI-020 | src/ui/InstanceSelector.h, .cpp, mainwindow.ui | Switch between Instances in UI | Manual UI test |
| MI-023 | Instance create/edit/delete dialogs | TODO | High | MI-022 | src/ui/InstanceDialog.h, .cpp, .ui | All fields editable, validation works | Manual UI test |
| MI-024 | Instance list persistence | TODO | High | MI-020 | InstanceManager.cpp | Survives app restart | Restart test |
| MI-025 | Instance rename preserves data | TODO | Medium | MI-020 | InstanceManager.cpp | UUID unchanged, all relations intact | Rename + verify test |

---

## Phase 3: Multiple Independent Core Processes

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-030 | Refactor CoreProcess for multi-instance | TODO | High | MI-020 | src/sys/Process.cpp, InstanceProcess.h | Multiple CoreProcess objects can run | Unit test |
| MI-031 | Unique RPC socket naming per Instance | TODO | High | MI-030 | src/sys/Process.cpp, RPC.h | `throne-instance-{uuid}` format | Socket name test |
| MI-032 | Implement ProcessSupervisor | TODO | High | MI-030 | src/instance/ProcessSupervisor.h, .cpp | Start/stop/restart per Instance | Integration test |
| MI-033 | RPCClientPool for connection management | TODO | High | MI-031 | src/api/RPCClientPool.h, .cpp | Routes calls to correct Instance | Multi-instance RPC test |
| MI-034 | HealthMonitor with crash detection | TODO | High | MI-032 | src/instance/HealthMonitor.h, .cpp | Tracks PID, RPC status, auto-restart | Crash simulation test |
| MI-035 | Run 2 Xray Instances simultaneously | TODO | High | MI-032, MI-033 | ProcessSupervisor.cpp | Both running, independent configs | Manual multi-instance test |

---

## Phase 4: Independent Ports & Instance Dashboard

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-040 | Implement PortAllocator service | TODO | High | MI-016 | src/instance/PortAllocator.h, .cpp | Allocates, tracks, releases ports | Unit test |
| MI-041 | Port conflict detection in UI | TODO | High | MI-040 | src/ui/InstanceDialog.cpp | Red warning on duplicate port entry | UI test |
| MI-042 | Runtime port bind verification | TODO | High | MI-040 | ProcessSupervisor.cpp | Fails gracefully if port taken | Bind failure test |
| MI-043 | Instance dashboard UI | TODO | Medium | MI-022 | src/ui/InstanceDashboard.h, .cpp, .ui | Shows status, PID, ports, traffic | Manual UI test |
| MI-044 | Per-Instance log filtering | TODO | Medium | MI-035 | src/ui/LogViewer.cpp | Log viewer filters by Instance | Log test |
| MI-045 | Port change applies without restart | TODO | Medium | MI-040, MI-032 | ProcessSupervisor.cpp | Running Instance updates config | Live port change test |

---

## Phase 5: Independent Subscriptions & Profiles

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-050 | Group UI scoped to current Instance | TODO | High | MI-022, MI-013 | src/ui/group/GroupWidget.cpp | Groups only show for active Instance | UI test |
| MI-051 | Subscription fetch per Instance | TODO | High | MI-050 | src/configs/sub/Subscription.cpp | Instance A fetch doesn't affect B | Multi-instance sub test |
| MI-052 | Profile operations per Instance | TODO | High | MI-050 | src/ui/profile/ProfileWidget.cpp | Add/remove/test profiles per Instance | UI test |
| MI-053 | Same URL in two Instances test | TODO | Medium | MI-051 | Subscription.cpp | Independent profile sets created | Dual-instance test |
| MI-054 | Delete Instance cascades Groups/Profiles | TODO | High | MI-020 | InstanceManager.cpp | Other Instances unaffected | Delete test |

---

## Phase 6: Startup, Auto-Start & State Recovery

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-060 | Auto-start setting per Instance | TODO | High | MI-010, MI-020 | Instance.h, InstanceManager.cpp | Enabled Instances start on launch | Startup test |
| MI-061 | Staggered startup (startup_delay_ms) | TODO | Medium | MI-060 | InstanceManager.cpp | Instances start with delays | Timing test |
| MI-062 | Restore last active profile | TODO | High | MI-060 | InstanceManager.cpp, Profile.cpp | Correct profile selected per Instance | Restart test |
| MI-063 | Crash detection & auto-restart | TODO | High | MI-034 | HealthMonitor.cpp | Restarts with 10s rate limit | Crash test |
| MI-064 | Clean exit handling | TODO | High | MI-063 | InstanceManager.cpp | No auto-restart on clean quit | Clean exit test |
| MI-065 | No duplicate processes on restart | TODO | High | MI-033 | RPCClientPool.cpp | PID/RPC check prevents duplicates | Restart test |

---

## Phase 7: Core Adapters (sing-box, External)

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-070 | CoreAdapter abstract base class | TODO | High | MI-030 | src/core_adapters/CoreAdapter.h | Interface defined, compilable | Compile test |
| MI-071 | XrayAdapter implementation | TODO | High | MI-070 | src/core_adapters/XrayAdapter.h, .cpp | Generates Xray config, starts process | Xray instance test |
| MI-072 | SingBoxAdapter implementation | TODO | High | MI-070 | src/core_adapters/SingBoxAdapter.h, .cpp | Generates sing-box config, starts process | sing-box instance test |
| MI-073 | ExternalCoreAdapter implementation | TODO | Medium | MI-070 | src/core_adapters/ExternalCoreAdapter.h, .cpp | Starts user binary, bridges via SOCKS | External core test |
| MI-074 | CoreAdapterFactory with detection | TODO | High | MI-071, MI-072 | src/core_adapters/CoreAdapterFactory.h, .cpp | Detects available cores on system | Detection test |
| MI-075 | Core capability matrix | TODO | Medium | MI-071, MI-072 | CoreAdapter.h | Protocol/Core compatibility table | Capability test |

---

## Phase 8: Core Selection for Chain & Profiles

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-080 | Add core_type to Chain outbound | TODO | High | MI-074 | include/configs/outbounds/chain.h | UI shows Core selector for Chain | UI test |
| MI-081 | Custom Xray Outbound forces Xray | TODO | High | MI-071 | include/configs/outbounds/custom.h | Xray config generated for Custom | Custom Xray test |
| MI-082 | ConfigGenerator routes to correct Adapter | TODO | High | MI-074 | src/configs/generate.cpp | Chain with Xray Core uses XrayAdapter | Chain test |
| MI-083 | Mixed Core Chain bridging | TODO | Medium | MI-082 | src/configs/generate.cpp | sing-box ↔ Xray via local SOCKS | Mixed chain test |
| MI-084 | Validation: reject incompatible combos | TODO | High | MI-080 | CoreAdapter.h, generate.cpp | Error shown for unsupported protocol | Validation test |

---

## Phase 9: Central Router (Future)

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-090 | CentralRouter configuration model | TODO | Low | MI-043 | src/routing/CentralRouter.h | Rules definable in UI | Model test |
| MI-091 | sing-box as router implementation | TODO | Low | MI-090 | src/routing/SingBoxRouter.cpp | Router config generates sing-box JSON | Router test |
| MI-092 | Rule evaluation engine | TODO | Low | MI-091 | src/routing/RuleEngine.cpp | Domain/IP matching works | Rule test |
| MI-093 | Health-aware routing | TODO | Low | MI-092, MI-034 | src/routing/RuleEngine.cpp | Down Instances skipped | Health routing test |
| MI-094 | Router UI (rules, priorities, fallback) | TODO | Low | MI-090 | src/ui/routing/RouterWidget.cpp | Visual rule editor | UI test |

---

## Phase 10: TUN & Cross-Platform (Future)

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-100 | TUN per Instance (Windows) | TODO | Low | MI-065 | src/sys/windows/TunAdapter.cpp | Wintun adapter per Instance | Windows TUN test |
| MI-101 | TUN per Instance (Linux) | TODO | Low | MI-065 | src/sys/linux/TunAdapter.cpp | tun device per Instance | Linux TUN test |
| MI-102 | TUN per Instance (macOS) | TODO | Low | MI-065 | src/sys/macos/TunAdapter.cpp | NetworkExtension per Instance | macOS TUN test |
| MI-103 | System Proxy single-owner model | TODO | Medium | MI-065 | src/sys/*/SystemProxy.cpp | Only one Instance controls SP | SP transfer test |
| MI-104 | Kill switch per Instance | TODO | Medium | MI-065 | src/sys/KillSwitch.cpp | Blocks traffic if Instance down | Kill switch test |

---

## Phase 11: Testing, Optimization & Packaging (Future)

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-110 | Throne feature regression suite | TODO | High | All prior | tests/ | All existing features work | Regression suite |
| MI-111 | Memory benchmark (5 Instances) | TODO | Medium | MI-035 | BUILD_AND_TEST.md | < 200 MB total | Benchmark |
| MI-112 | CPU benchmark (5 Instances idle) | TODO | Medium | MI-035 | BUILD_AND_TEST.md | < 2% total | Benchmark |
| MI-113 | Startup time benchmark | TODO | Medium | MI-065 | BUILD_AND_TEST.md | < 10s for 5 auto-start | Benchmark |
| MI-114 | Windows build & installer | TODO | High | MI-110 | cmake/windows/ | .exe + portable ZIP | Build test |
| MI-115 | Linux build (AppImage, deb) | TODO | High | MI-110 | cmake/linux/ | Packages install & run | Build test |
| MI-116 | macOS build & notarization | TODO | High | MI-110 | cmake/macos/ | .app runs without quarantine | Build test |
| MI-117 | Complete all documentation | TODO | High | All | project-docs/ | All docs current | Doc review |

---

## Notes

- Tasks marked with `DONE` or `VERIFIED` have been completed in this session
- Tasks in `IN_PROGRESS` are currently being worked on
- Priority: High = blocks other tasks, Medium = important but not blocking, Low = future phase
- Dependencies show which tasks must complete first
- Acceptance criteria should be measurable and testable
- Test method indicates how to verify completion

---

## Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 0.1 | 2026-10-10 | Initial | Draft task list from roadmap |