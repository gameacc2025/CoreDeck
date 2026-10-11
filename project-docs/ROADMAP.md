# CoreDeck Development Roadmap

**Version:** 0.2  
**Status:** ACTIVE  
**Last Updated:** 2026-10-10  
**Rules:** Phases and milestones only. Task IDs and statuses live in `TASKS.md` (DEC-014). Scope tiers live in `MVP_SCOPE.md`. Platforms: Windows + Linux (DEC-012). No fixed calendar weeks; the AI proceeds autonomously (DEC-013).

---

## Milestones

| Milestone | Meaning | Status |
|-----------|---------|--------|
| M0 | Toolchain or CI ready, first configure/build result recorded | NOT REACHED |
| M1 | Needed Throne code integrated, CoreDeck builds and launches without any `Throne/` folder, initial tests run | NOT REACHED |
| M2 | Basic proxy workflow: create Instance/profile, start/stop core, local proxy works, port conflicts handled, state persists | NOT REACHED |
| M3 | Prototype gate (see AI_MASTER_PLAN section 11): P0 complete, tests pass, smoke passes, Windows ZIP + Linux CI artifact | NOT REACHED |
| M4 | First release with installer/packages and user docs | FUTURE |

## Phase Overview

| Phase | Name | Tier | Status | Depends on |
|-------|------|------|--------|------------|
| 0 | Analysis & documentation | — | DONE (docs verified; baseline build superseded) | — |
| 0b | Bootstrap: toolchain/CI, integrate Throne, first build, run existing tests, fix migration, remove Throne folder | P0 | TODO — start here | 0 |
| 1 | Instance data model & storage | P0 | CODE WRITTEN, UNVERIFIED | 0b |
| 2 | Default Instance & multi-Instance management | P0 | MI-020 code written, unverified; rest TODO | 1 |
| 3 | Multiple independent Core processes | P0 | TODO | 2 |
| 4 | Ports (PortAllocator, conflicts) and dashboard | P0 (allocator, conflicts) / P1 (dashboard, log filter, live change) | TODO | 3 |
| 5 | Independent subscriptions & profiles | P0 | TODO | 2 |
| 6 | Startup, auto-start, state recovery | P0 (auto-start, restore, clean exit, no duplicates) / P1 (stagger, restart tuning) | TODO | 3, 5 |
| 7 | Core adapters (Xray minimum is P0; sing-box, External are P1) | P0/P1 | TODO | 3 |
| 8 | Chain/profile core selection | P1 | TODO | 7 |
| 9 | Central Router | P2 | TODO | 4 |
| 10 | TUN, kill switch (Windows, Linux) | P2 | TODO | 4 |
| 11 | Tests, benchmarks, packaging | P0 (ZIP, smoke) / P1 (rest) | TODO | all P0 |

---

## Phase 0b: Bootstrap (do first)

Objectives: provision toolchain or CI; copy the required Throne code into CoreDeck's tree with its licence headers; create the root CMake; first build and launch; register and run the existing tests; fix the migration (nullable `groups.instance_id`, backup, `PRAGMA foreign_keys`, fixture tests); remove the temporary `Throne/` folder after path and content verification; freeze `MVP_SCOPE.md`.

Exit: M0 and M1 reached; `ctest` executed with recorded counts; tracked files contain no `Throne/` path references.

## Phase 1: Instance Data Model & Storage

Instance entity, `instances` / `instance_settings` / `port_allocations` tables, `groups.instance_id`, repositories, Throne to CoreDeck migration.  
Exit: migration tested on fresh, Throne-format, partial and failing DBs; CRUD verified; no data loss; Throne-readability of the migrated DB checked.

## Phase 2: Default Instance & Management

InstanceManager, Default Instance on first run, selector UI, create/edit/delete dialogs, persistence, rename preserving data.  
Exit: 5+ Instances can be created; switching Instances updates views; data survives restart.

## Phase 3: Independent Core Processes

CoreProcess refactor for multiple processes, unique RPC socket per Instance (`throne-instance-{uuid}` naming kept for compatibility), ProcessSupervisor, RPCClientPool, HealthMonitor, two Xray Instances at once.  
Exit: separate PIDs/ports/sockets/logs; stopping or crashing one leaves the other running.

## Phase 4: Ports

PortAllocator with real bind check, UI conflict warning, runtime bind verification (P0); dashboard, per-Instance log filter, live port change (P1).  
Strategy: SOCKS 10808, HTTP 10809, API 10810, +10 per Instance, range 10808–11000 (about 19 Instances; range configurable).  
Exit: duplicate port rejected in UI and at start with a clear error; GUI stays alive.

## Phase 5: Subscriptions & Profiles

Groups scoped to the current Instance, subscription fetch per Instance, same URL in two Instances independent, delete cascades only the Instance's own data.  
Exit: A and B independent; deleting A leaves B intact.

## Phase 6: Startup & Recovery

Auto-start, last-profile restore, clean exit vs crash, no duplicate processes on restart (P0); staggered start and crash-restart tuning with 10 s rate limit (P1).

## Phase 7: Core Adapters

CoreAdapter base class, XrayAdapter, factory with core detection (P0 minimum); SingBoxAdapter, ExternalCoreAdapter, capability matrix (P1).

## Phase 8: Chain & Profile Core Selection (P1)

`core_type` on Chain, Custom Xray forces Xray, config generator routes to the adapter, mixed-core bridging via local SOCKS, validation of incompatible combinations.

## Phase 9: Central Router (P2)

Router model, sing-box as router, rule engine, health-aware routing, router UI; independent ports keep working if the router fails; no routing loops.

## Phase 10: TUN & Kill Switch (P2, Windows + Linux)

Per-Instance TUN (Wintun / tun device), system-proxy single-owner model, kill switch.

## Phase 11: Tests, Benchmarks, Packaging

P0: Windows portable ZIP, Linux CI binary, smoke tests, licence bundle, docs.  
P1: Throne regression suite, benchmarks (5 Instances: < 200 MB, < 2% CPU, < 10 s startup), installer, AppImage/deb/rpm.

---

## Parallelization

| Phases | Parallel? | Reason |
|--------|-----------|--------|
| 3 + 5 | Yes | Core processes independent of subscription UI |
| 7 (Xray) + 3 | Partly | Xray adapter needed by the process supervisor |
| 8 after 7 | No | Needs adapters |
| 9, 10 after P0 | No | Router/TUN need stable Local Proxy Mode |

## Risk Register

| Risk | Impact | Likelihood | Mitigation |
|------|--------|------------|------------|
| Toolchain stays unprovisioned | Critical | Medium | Self-install via winget/aqt; fall back to GitHub Actions CI |
| Throne integration drags in unneeded code | Medium | Medium | Copy only needed modules; no macOS code; review diff |
| Port conflicts at scale | High | Medium | PortAllocator + bind-time verification |
| Core resource exhaustion | High | Low | HealthMonitor limits |
| Migration data loss | Critical | Low | Backup file, transaction, fixtures, rollback tests |
| TUN complexity | High | High | Deferred to P2 |
| RPC socket cleanup on crash | Medium | Medium | RAII + startup cleanup |

---

## Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 0.1 | 2026-10-10 | Initial | Draft roadmap |
| 0.2 | 2026-10-10 | Review | Removed colliding task IDs, added Phase 0b and milestones M0–M4, tiers, dropped macOS, removed false "baseline build verified" tick |
