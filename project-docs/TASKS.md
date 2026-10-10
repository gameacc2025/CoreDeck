# CoreDeck Task List

**Version:** 0.2  
**Status:** ACTIVE (single source of task IDs, DEC-014)  
**Last Updated:** 2026-10-10

---

## Task Status Legend

| Status | Meaning |
|--------|---------|
| `TODO` | Not started |
| `IN_PROGRESS` | Currently being worked on |
| `BLOCKED` | Waiting on dependency or decision |
| `DONE_UNVERIFIED` | Code written, but not built or tests not executed (static review does not count) |
| `VERIFIED` | Built and relevant tests actually executed and passed (local run or cited CI run) |
| `SUPERSEDED` | Replaced by another task (named) |
| `DROPPED` | Out of scope by decision (named) |

Scope tiers (P0/P1/P2) per task are defined in `MVP_SCOPE.md`. Do P0 first.

---

## Phase 0: Repository Analysis & Baseline

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-000 | Clone Throne repository | VERIFIED | High | — | Throne/ | Repository cloned locally | `git status` in Throne/ |
| MI-001 | Analyze Throne architecture | VERIFIED | High | MI-000 | ARCHITECTURE.md | Architecture documented | Review ARCHITECTURE.md |
| MI-002 | Verify baseline build | SUPERSEDED | High | MI-000 | BUILD_AND_TEST.md | Replaced by MI-118 (toolchain) and MI-120 (first build of integrated CoreDeck) | — |
| MI-003 | Create project documentation structure | VERIFIED | High | — | project-docs/ | All doc files created | `ls project-docs/` |
| MI-004 | Write PROJECT_SPEC.md | VERIFIED | High | MI-001 | PROJECT_SPEC.md | Complete requirements documented | Review spec |
| MI-005 | Write ARCHITECTURE.md | VERIFIED | High | MI-001 | ARCHITECTURE.md | Current + target architecture documented | Review arch |
| MI-006 | Write ROADMAP.md | VERIFIED | High | MI-004 | ROADMAP.md | Phases and tasks defined | Review roadmap |
| MI-007 | Write AI_WORKFLOW.md | VERIFIED | High | — | AI_WORKFLOW.md | Development rules documented | Review workflow |
| MI-008 | Write DEPENDENCIES_AND_LICENSES.md | VERIFIED | High | MI-000 | DEPENDENCIES_AND_LICENSES.md | All licenses documented | Review licenses |

---

## Phase 0b: Bootstrap to First Build (Milestones M0, M1) — DO THIS FIRST

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-118 | Provision toolchain or CI (M0) | TODO | P0 | — | BUILD_AND_TEST.md, .github/workflows/build.yml | CMake, Ninja, compiler, Qt present locally, OR a CI workflow builds on windows-latest and ubuntu-latest; first configure/build result recorded | `cmake --version`, `qmake -v`, or green CI run cited |
| MI-119 | Integrate required Throne code into CoreDeck tree + root CMakeLists.txt | TODO | P0 | MI-118 | src/, include/, 3rdparty/, res/, CMakeLists.txt, DEPENDENCIES_AND_LICENSES.md | Required Throne files copied (no macOS-only code, no build output), licence headers kept, own root CMake, no path to `Throne/` | `grep -ri "Throne/" CMakeLists.txt cmake/ script/` shows nothing; configure succeeds |
| MI-120 | First successful build and launch (M1) | TODO | P0 | MI-119 | BUILD_AND_TEST.md | Compiles on Windows (local or CI) and Linux (CI); app starts and shows the main window | build log + launch evidence recorded |
| MI-121 | Build and run existing tests (MI-010..MI-020), fix failures | TODO | P0 | MI-120 | tests/, CMake tests | `test_instance`, `test_instancemanager` registered in CTest, executed; failures fixed; static-only tests rewritten to test real behavior | `ctest --output-on-failure` counts recorded |
| MI-122 | Fix migration: nullable groups.instance_id, backup, foreign_keys pragma, fixture tests | TODO | P0 | MI-121 | src/database/Migration.cpp, tests/ | Per DEC-015 and Master Plan section 8; fresh, Throne-format, partial, rollback cases tested; Throne-readability of migrated DB checked | migration tests pass in CTest |
| MI-123 | Remove temporary Throne folder and stale references | TODO | P0 | MI-120, MI-121 | repo-wide | Exact path verified, no owner work inside, folder removed; tracked files contain no sibling/absolute Throne path; build still passes | grep + clean-clone build |
| MI-124 | Write MVP_SCOPE.md and freeze P0 | VERIFIED | P0 | — | MVP_SCOPE.md | P0/P1/P2 listed and mapped to task IDs | document review |
| MI-125 | Xray core acquisition/location and launch check | TODO | P0 | MI-120 | core manager code | Xray binary found or downloaded from official source; missing-binary error is clear; version recorded | unit test + manual |
| MI-126 | End-to-end smoke test S1/S2 and multi-instance MI-1..MI-3 | TODO | P0 | MI-035, MI-042 | BUILD_AND_TEST.md | Smoke results recorded honestly | manual + scripted |

---

## Phase 1: Instance Data Model & Storage

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-010 | Design Instance C++ class | DONE_UNVERIFIED | High | MI-120 | include/database/entities/Instance.h, src/database/entities/Instance.cpp, tests/test_instance.cpp | Class with all fields, JSON serialization | Unit test |
| MI-011 | Add Instance tables to Database.cpp | DONE_UNVERIFIED | High | MI-010 | include/database/Migration.h, src/database/Migration.cpp, src/database/DatabaseManager.cpp | Migration creates tables on first run | Migration test |
| MI-012 | Implement InstanceRepository | DONE_UNVERIFIED | High | MI-011 | include/database/InstanceRepository.h, src/database/InstanceRepository.cpp | CRUD operations work | Unit test |
| MI-013 | Add instance_id to Group entity | DONE_UNVERIFIED | High | MI-011 | Migration adds instance_id column to groups table | Integration test |
| MI-014 | Write migration logic (Throne → CoreDeck) | DONE_UNVERIFIED | High | MI-011 | src/database/Migration.cpp | Existing data moves to Default Instance | Migration test with real DB |
| MI-015 | Add InstanceSettings table & repo | DONE_UNVERIFIED | Medium | MI-011 | Migration creates instance_settings table | Unit test |
| MI-016 | Add PortAllocations table & repo | DONE_UNVERIFIED | Medium | MI-011 | Migration creates port_allocations table | Unit test |

---

## Phase 2: Default Instance & Multi-Instance Management

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-020 | Implement InstanceManager singleton | DONE_UNVERIFIED | High | MI-012 | include/instance/InstanceManager.h, src/instance/InstanceManager.cpp, tests/test_instancemanager.cpp | Manages Instance lifecycle | Unit test (never executed; static review only) |
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

## Phase 7: Core Adapters (sing-box, External)  — P1 (XrayAdapter MI-070/071/074 may be pulled into P0 if needed)

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-070 | CoreAdapter abstract base class | TODO | High | MI-030 | src/core_adapters/CoreAdapter.h | Interface defined, compilable | Compile test |
| MI-071 | XrayAdapter implementation | TODO | High | MI-070 | src/core_adapters/XrayAdapter.h, .cpp | Generates Xray config, starts process | Xray instance test |
| MI-072 | SingBoxAdapter implementation | TODO | High | MI-070 | src/core_adapters/SingBoxAdapter.h, .cpp | Generates sing-box config, starts process | sing-box instance test |
| MI-073 | ExternalCoreAdapter implementation | TODO | Medium | MI-070 | src/core_adapters/ExternalCoreAdapter.h, .cpp | Starts user binary, bridges via SOCKS | External core test |
| MI-074 | CoreAdapterFactory with detection | TODO | High | MI-071, MI-072 | src/core_adapters/CoreAdapterFactory.h, .cpp | Detects available cores on system | Detection test |
| MI-075 | Core capability matrix | TODO | Medium | MI-071, MI-072 | CoreAdapter.h | Protocol/Core compatibility table | Capability test |

---

## Phase 8: Core Selection for Chain & Profiles  — P1

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-080 | Add core_type to Chain outbound | TODO | High | MI-074 | include/configs/outbounds/chain.h | UI shows Core selector for Chain | UI test |
| MI-081 | Custom Xray Outbound forces Xray | TODO | High | MI-071 | include/configs/outbounds/custom.h | Xray config generated for Custom | Custom Xray test |
| MI-082 | ConfigGenerator routes to correct Adapter | TODO | High | MI-074 | src/configs/generate.cpp | Chain with Xray Core uses XrayAdapter | Chain test |
| MI-083 | Mixed Core Chain bridging | TODO | Medium | MI-082 | src/configs/generate.cpp | sing-box ↔ Xray via local SOCKS | Mixed chain test |
| MI-084 | Validation: reject incompatible combos | TODO | High | MI-080 | CoreAdapter.h, generate.cpp | Error shown for unsupported protocol | Validation test |

---

## Phase 9: Central Router (Future) — P2

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-090 | CentralRouter configuration model | TODO | Low | MI-043 | src/routing/CentralRouter.h | Rules definable in UI | Model test |
| MI-091 | sing-box as router implementation | TODO | Low | MI-090 | src/routing/SingBoxRouter.cpp | Router config generates sing-box JSON | Router test |
| MI-092 | Rule evaluation engine | TODO | Low | MI-091 | src/routing/RuleEngine.cpp | Domain/IP matching works | Rule test |
| MI-093 | Health-aware routing | TODO | Low | MI-092, MI-034 | src/routing/RuleEngine.cpp | Down Instances skipped | Health routing test |
| MI-094 | Router UI (rules, priorities, fallback) | TODO | Low | MI-090 | src/ui/routing/RouterWidget.cpp | Visual rule editor | UI test |

---

## Phase 10: TUN & Cross-Platform (Future) — P2 (Windows + Linux only)

| ID | Title | Status | Priority | Dependencies | Related Files | Acceptance Criteria | Test Method |
|----|-------|--------|----------|--------------|---------------|---------------------|-------------|
| MI-100 | TUN per Instance (Windows) | TODO | Low | MI-065 | src/sys/windows/TunAdapter.cpp | Wintun adapter per Instance | Windows TUN test |
| MI-101 | TUN per Instance (Linux) | TODO | Low | MI-065 | src/sys/linux/TunAdapter.cpp | tun device per Instance | Linux TUN test |
| MI-102 | TUN per Instance (macOS) | DROPPED | — | — | — | Dropped by DEC-012 | — |
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
| MI-116 | macOS build & notarization | DROPPED | — | — | — | Dropped by DEC-012 | — |
| MI-117 | Complete all documentation | TODO | High | All | project-docs/ | All docs current | Doc review |

---

## Notes

- Only `VERIFIED` means finished. `DONE_UNVERIFIED` code has never been built or tested
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
| 0.2 | 2026-10-10 | Review | Honest statuses (DEC-014), bootstrap phase 0b (MI-118..MI-126), macOS dropped (DEC-012) |