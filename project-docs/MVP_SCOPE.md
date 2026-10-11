# CoreDeck MVP Scope

**Version:** 1.0  
**Status:** FROZEN (changes only with a documented reason in DECISIONS.md)  
**Last Updated:** 2026-10-10  
**Goal:** a testable prototype for the owner: builds, launches, runs two independent proxy Instances end to end on Windows (Linux build verified in CI).

Tier meaning: **P0** = required for the prototype, **P1** = right after, **P2** = future. Task IDs refer to `TASKS.md`.

---

## P0 — Required for the prototype

| # | Requirement | Tasks |
|---|-------------|-------|
| 1 | Toolchain or CI ready; first build recorded (M0) | MI-118 |
| 2 | Needed Throne code integrated; own root CMake; no `Throne/` dependency (M1) | MI-119, MI-123 |
| 3 | App builds (Windows local/CI, Linux CI) and launches | MI-120 |
| 4 | Existing Instance model, repository, manager tests built, executed, fixed | MI-010..MI-016, MI-020, MI-121 |
| 5 | Safe in-place Throne DB migration (backup, nullable `groups.instance_id`, FK pragma, fixtures) | MI-011, MI-013, MI-014, MI-122 |
| 6 | Default Instance auto-created; Instance list persists; rename keeps data | MI-021, MI-024, MI-025 |
| 7 | Instance selector and create/edit/delete dialogs | MI-022, MI-023 |
| 8 | Several independent Core processes (own PID, config, log, RPC socket) | MI-030, MI-031, MI-032, MI-033 |
| 9 | Xray core located/downloaded, started, monitored, stopped; clear missing-binary error | MI-125, MI-070, MI-071 (minimum Xray adapter) |
| 10 | PortAllocator with real bind check; conflict errors in UI and at start | MI-040, MI-041, MI-042 |
| 11 | Per-Instance groups/subscriptions/profiles independent; delete cascades only its own data | MI-050..MI-054 |
| 12 | Crash isolation; clean shutdown with no orphan cores; no duplicate processes on restart | MI-034, MI-064, MI-065 |
| 13 | Per-Instance auto-start and last-profile restore | MI-060, MI-062 |
| 14 | Local proxy works end to end (SOCKS/HTTP per Instance); system proxy single-owner behaviour as inherited | MI-035, MI-103 |
| 15 | Logging without secrets; actionable errors; GUI never crashes on core failure | across tasks |
| 16 | Critical unit/integration tests run in CTest | all |
| 17 | Smoke + multi-instance tests recorded | MI-126 |
| 18 | Windows portable ZIP + Linux CI artifact + docs; GPL notices | MI-114 (ZIP), MI-115 (CI binary only), MI-117 |

## P1 — After the prototype

- sing-box and External adapters, capability matrix, adapter factory (MI-072..MI-075)
- Chain/profile core selection and mixed-core bridging (MI-080..MI-084)
- Staggered startup, crash auto-restart refinements (MI-061, MI-063)
- Instance dashboard, per-Instance log filtering, live port change (MI-043..MI-045)
- Linux AppImage/deb/rpm, Windows installer, benchmarks (MI-111..MI-113)
- Full Throne regression suite (MI-110)

## P2 — Future

- Central Router (MI-090..MI-094)
- Per-Instance TUN (Windows, Linux) and kill switch (MI-100, MI-101, MI-104)
- Remote API, web UI, plugin ecosystem, traffic shaping

## Out of scope

- macOS (DEC-012), mobile
- Custom updater/telemetry (unless a later decision adds them)

---

## Rules

- Do not start P1/P2 while any P0 item is broken, unless it is a prerequisite.
- Do not refactor unrelated modules during a P0 task.
- Prefer reuse and adaptation of the integrated Throne code over rewriting.
- A P0 item is done only when its task is `VERIFIED`.
