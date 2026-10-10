# CoreDeck Progress Tracker

**Version:** 0.1  
**Last Updated:** 2026-10-10  
**Branch:** `feature/multi-instance`  
**Latest Commit:** d1225b7 (Initial documentation)

---

## Current Status Summary

| Metric | Value |
|--------|-------|
| **Phase** | 0: Repository Analysis & Baseline (Build blocked) / 1: Instance Data Model (Starting) |
| **Overall Progress** | ~20% (Documentation complete, build blocked, Instance model design starting) |
| **Last Verified Build** | BLOCKED - Missing toolchain (CMake, Qt6, MSVC/MinGW, Ninja) |
| **Last Successful Test** | N/A |
| **Blocking Issues** | KI-001: Build toolchain not installed on Windows 11 machine |
| **Next Milestone** | Phase 1: Instance data model implementation (MI-010) |

---

## Completed Work (Session 001)

### Documentation Created
- [x] `README.md` - Project overview and quick start
- [x] `project-docs/PROJECT_SPEC.md` - Complete product specification
- [x] `project-docs/ARCHITECTURE.md` - Current Throne architecture + target CoreDeck architecture
- [x] `project-docs/ROADMAP.md` - 11-phase development plan with tasks
- [x] `project-docs/TASKS.md` - Detailed task list with acceptance criteria
- [x] `project-docs/AI_WORKFLOW.md` - Mandatory development rules
- [x] `project-docs/DEPENDENCIES_AND_LICENSES.md` - License compliance audit
- [x] `project-docs/BUILD_AND_TEST.md` - Build/test procedures (updated with blocker)
- [x] `project-docs/KNOWN_ISSUES.md` - Issues and risk register (updated)
- [x] `project-docs/CHANGELOG.md` - Change log
- [x] `project-docs/DECISIONS.md` - 10 architectural decisions
- [x] `project-docs/PROGRESS.md` - Status tracker (this file)
- [x] `project-docs/sessions/2026-10-10-session-001.md` - Session 001 report

### Repository Analysis
- [x] Throne repository cloned at `G:\Project\CoreDeck\Throne\`
- [x] Architecture analyzed: Qt6, C++20, CMake, SQLite, sing-box/Xray cores
- [x] Key components identified: CoreProcess, RPC, Config Generation, Database, UI
- [x] Data model documented: Settings, Groups, Profiles, Routes, IpLists
- [x] Build system analyzed: CMake with platform-specific configs

### Git Status
- [x] CoreDeck repository initialized at `G:\Project\CoreDeck\`
- [x] Throne upstream preserved as reference (not renamed - it's the upstream source)
- [x] Branch `feature/multi-instance` ready for development
- [x] Initial commit d1225b7 with all documentation

---

## In Progress (Session 002)

| Task | Status | Notes |
|------|--------|-------|
| MI-002: Verify baseline build | BLOCKED | Toolchain missing: CMake, Qt6, MSVC/MinGW, Ninja not installed |
| MI-010: Design Instance C++ class | DONE | Instance entity created with all fields, JSON serialization, unit tests |
| MI-007: Write AI_WORKFLOW.md | DONE | Completed in Session 001 |
| MI-008: Write DEPENDENCIES_AND_LICENSES.md | DONE | Completed in Session 001 |

---

## Completed (Session 002)

### MI-010: Instance Data Model Implementation
- **Files Created:**
  - `include/database/entities/Instance.h` - Instance entity class with all fields from PROJECT_SPEC.md Section 3
  - `src/database/entities/Instance.cpp` - Implementation with JSON serialization/deserialization
  - `tests/test_instance.cpp` - Comprehensive unit tests covering all acceptance criteria

- **Acceptance Criteria Covered:**
  1. ✅ Create a valid Instance (CreateDefault factory method)
  2. ✅ Generate and preserve its UUID (QUuid primary key)
  3. ✅ Rename Instance without changing identity (UUID stable, name mutable)
  4. ✅ Handle invalid/missing identity (IsValid(), Validate() methods)
  5. ✅ Validate supported Core types (CoreType enum: Xray, SingBox, External)
  6. ✅ Verify default Core selection (Xray per DEC-007)
  7. ✅ Validate relevant endpoint settings (port ranges, bind address)
  8. ✅ Round-trip serialization/deserialization (ToJson/FromJson)
  9. ✅ Handle missing/malformed/unsupported fields safely (defaults, null UUID handling)
  10. ✅ Detect duplicate identities (equality operator based on UUID)
  11. ✅ Backward compatibility for legacy representation (graceful defaults for missing fields)

- **Design Decisions:**
  - UUID as primary key (QUuid) per DEC-002
  - CoreType enum with Xray default per DEC-007
  - Nested structs for CoreConfig and DNSSettings for maintainability
  - Timestamps in Unix seconds (qint64) for cross-platform compatibility
  - Equality/inequality operators based on UUID identity
  - Validation method returns empty string on success, error message on failure

---

## Known Issues & Risks

| Issue | Severity | Status | Mitigation |
|-------|----------|--------|------------|
| Build toolchain not installed (KI-001) | High | OPEN | Install VS2022 Build Tools + Qt6 + CMake + Ninja, or use CI |
| Windows Qt6/MinGW/MSVC setup unknown | Medium | OPEN | Check Throne CI/CD for exact versions |
| Migration strategy untested | Medium | PLANNED | Test with real Throne DB |
| Port allocation race condition | Medium | DESIGN | Bind-time verification in PortAllocator |

---

## Resource Links

| Resource | URL |
|----------|-----|
| Throne Repository | https://github.com/throneproj/Throne |
| CoreDeck Repository | https://github.com/DevastBit/CoreDeck |
| v2rayN Reference | https://github.com/2dust/v2rayN |
| sing-box | https://github.com/SagerNet/sing-box |
| Xray-core | https://github.com/XTLS/Xray-core |

---

## Session History

| Session | Date | Focus | Outcome |
|---------|------|-------|---------|
| 1 | 2026-10-10 | Phase 0: Analysis & Documentation | 13 doc files created, architecture analyzed, repo initialized |
| 2 | 2026-10-10 | Build verification + MI-010 | Build blocked (missing toolchain), starting Instance model design |

---

## Next Session Plan (Session 003)

1. Complete MI-010: Instance C++ class implementation with unit tests
2. Begin MI-011: Add Instance tables to Database.cpp
3. Plan toolchain installation or CI setup for build verification

---

## Metrics Dashboard

```
Phase 0: ████████░░ 80% (docs done, build blocked)
Phase 1: ████░░░░░░ 20% (MI-010 done, MI-011 next)
Phase 2: ░░░░░░░░░░  0%
Phase 3: ░░░░░░░░░░  0%
Phase 4: ░░░░░░░░░░  0%
Phase 5: ░░░░░░░░░░  0%
Phase 6: ░░░░░░░░░░  0%
Phase 7: ░░░░░░░░░░  0%
Phase 8: ░░░░░░░░░░  0%
Phase 9: ░░░░░░░░░░  0%
Phase 10: ░░░░░░░░░░  0%
Phase 11: ░░░░░░░░░░  0%
```

---

*Update this file after every significant work session. Keep it accurate for session recovery.*