# CoreDeck Progress Tracker

**Version:** 0.1  
**Last Updated:** 2026-10-10  
**Branch:** `feature/multi-instance`  
**Latest Commit:** (none yet - initial documentation)

---

## Current Status Summary

| Metric | Value |
|--------|-------|
| **Phase** | 0: Repository Analysis & Baseline |
| **Overall Progress** | ~15% (Documentation setup) |
| **Last Verified Build** | Not yet attempted |
| **Last Successful Test** | N/A |
| **Blocking Issues** | None |
| **Next Milestone** | Phase 0 completion: Baseline build verified |

---

## Completed Work (This Session)

### Documentation Created
- [x] `README.md` - Project overview and quick start
- [x] `project-docs/PROJECT_SPEC.md` - Complete product specification
- [x] `project-docs/ARCHITECTURE.md` - Current Throne architecture + target CoreDeck architecture
- [x] `project-docs/ROADMAP.md` - 11-phase development plan with tasks
- [x] `project-docs/TASKS.md` - Detailed task list with acceptance criteria

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

---

## In Progress

| Task | Status | Notes |
|------|--------|-------|
| MI-002: Verify baseline build | IN_PROGRESS | Need to run CMake build on Throne |
| MI-007: Write AI_WORKFLOW.md | TODO | Next |
| MI-008: Write DEPENDENCIES_AND_LICENSES.md | TODO | After AI_WORKFLOW |

---

## Upcoming (Next Session)

### Immediate Next Steps
1. **Build Throne baseline** - Run CMake configure and build to verify environment
2. **Document build results** - Update BUILD_AND_TEST.md with actual commands/output
3. **Complete remaining Phase 0 docs** - AI_WORKFLOW.md, DEPENDENCIES_AND_LICENSES.md
4. **Create first session report** - Document this session in `project-docs/sessions/`

### Phase 1 Preparation
- Design Instance C++ class structure
- Plan database migration strategy
- Identify all files needing modification for Instance support

---

## Known Issues & Risks

| Issue | Severity | Status | Mitigation |
|-------|----------|--------|------------|
| Build environment not verified | High | Open | Run CMake build next |
| Windows Qt6/MinGW/MSVC setup unknown | Medium | Open | Check Throne CI/CD for hints |
| Migration strategy untested | Medium | Planned | Test with real Throne DB |

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
| 1 | 2026-10-10 | Phase 0: Analysis & Documentation | 5 doc files created, architecture analyzed, repo initialized |

---

## Next Session Plan

1. Run Throne baseline build (CMake configure + build)
2. Document build commands and results in BUILD_AND_TEST.md
3. Complete AI_WORKFLOW.md and DEPENDENCIES_AND_LICENSES.md
4. Create session report for 2026-10-10
5. Begin Phase 1: Instance data model design

---

## Metrics Dashboard

```
Phase 0: ████████░░ 80% (docs done, build pending)
Phase 1: ░░░░░░░░░░  0%
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