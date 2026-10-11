# CoreDeck Known Issues

**Version:** 0.2  
**Last Updated:** 2026-10-10

---

## Active Issues

### Build & Environment
| ID | Issue | Severity | Status | Reproduction | Expected | Actual |
|----|-------|----------|--------|--------------|----------|--------|
| KI-001 | No toolchain; nothing ever built | Critical | OPEN | `cmake --version` | Build works | CMake, Qt6, MSVC/MinGW, Ninja missing on Windows 11. AI to self-install or use CI (MI-118) |
| KI-013 | CoreDeck has no root CMakeLists.txt and Throne code not integrated | Critical | OPEN | configure CoreDeck root | Self-contained build | Written code includes headers that live only in the Throne folder (MI-119) |

### Architecture & Design
| ID | Issue | Severity | Status | Reproduction | Expected | Actual |
|----|-------|----------|--------|--------------|----------|--------|
| KI-002 | Migration code written but never run | High | OPEN | Run migration on Throne-format fixture | Seamless migration | Unverified (MI-122) |
| KI-014 | `groups.instance_id` NOT NULL DEFAULT placeholder + FK invalid in SQLite | High | OPEN | Run ALTER TABLE | Nullable + backfill (DEC-015) | Design flaw in Migration.cpp/ARCHITECTURE (MI-122) |
| KI-015 | `PRAGMA foreign_keys` not guaranteed ON | High | OPEN | Delete Instance | Cascade works | Must be set per connection (MI-122) |
| KI-016 | Throne readability of migrated DB assumed, not tested | Medium | OPEN | Open migrated DB with Throne-format queries | Works | Unverified |
| KI-003 | Port allocation race condition | Medium | DESIGN | Two Instances start simultaneously, same port | Bind-time check catches it | Not implemented |

### Core Management
| ID | Issue | Severity | Status | Reproduction | Expected | Actual |
|----|-------|----------|--------|--------------|----------|--------|
| KI-004 | Core binary auto-download | Medium | OPEN | First run without cores | Auto-download sing-box/Xray | Throne handles this |
| KI-005 | Xray vs sing-box binary detection | Medium | PLANNED | Multiple cores installed | Detect and offer choice | Not implemented |

### Cross-Platform
| ID | Issue | Severity | Status | Reproduction | Expected | Actual |
|----|-------|----------|--------|--------------|----------|--------|
| KI-006 | Windows Wintun driver install | High | FUTURE | Enable TUN on Windows | Silent install | Requires admin |
| KI-007 | Linux capabilities for TUN | High | FUTURE | Enable TUN on Linux | Works without full root | Needs CAP_NET_ADMIN |
| KI-008 | macOS NetworkExtension entitlements | — | WONT_FIX | — | — | macOS dropped (DEC-012) |

### Data & Persistence
| ID | Issue | Severity | Status | Reproduction | Expected | Actual |
|----|-------|----------|--------|--------------|----------|--------|
| KI-009 | Settings migration completeness | Medium | OPEN | Migrate Throne DB | All settings preserved | Code written, not tested |
| KI-010 | Instance delete cascade | High | OPEN | Delete Instance with Groups | Only Instance data removed | Depends on KI-015; not tested |

### UI/UX
| ID | Issue | Severity | Status | Reproduction | Expected | Actual |
|----|-------|----------|--------|--------------|----------|--------|
| KI-011 | Instance selector UX | Medium | PLANNED | 10+ Instances | Usable dropdown/list | Not designed |
| KI-012 | Per-Instance log filtering | Medium | PLANNED | Multiple Instances running | Filter logs by Instance | Not implemented |

---

## Resolved / Not Applicable

| ID | Issue | Resolution |
|----|-------|------------|
| — | None yet | — |

---

## Risk Register

| Risk | Impact | Likelihood | Mitigation |
|------|--------|------------|------------|
| Port exhaustion at high Instance count | High | Low | PortAllocator with 10808-11000 range, user can extend |
| Core process resource exhaustion | High | Low | HealthMonitor with CPU/memory limits per Instance |
| Migration data loss | Critical | Low | Automated tests with real Throne DBs, backup before migrate |
| Cross-platform TUN complexity | High | High | Defer to Phase 10, use sing-box abstraction layer |
| RPC socket cleanup on crash | Medium | Medium | RAII in CoreProcess, HealthMonitor cleanup on startup |
| Qt6 version compatibility | Medium | Medium | Pin Qt version in CI, test multiple versions |
| Toolchain stays missing | Critical | Medium | Self-install via winget/aqt, else GitHub Actions |
| Autonomous AI reports unverified results | High | Medium | DONE_UNVERIFIED status, evidence required, final report separates verified vs unverified |
| sing-box/Xray API breaking changes | Medium | Medium | Pin core versions, test on update |

---

## Test Gaps

| Area | Current Coverage | Needed |
|------|------------------|--------|
| Database migration | None | Unit + integration tests with real DBs |
| Multi-Core process management | None | ProcessSupervisor tests |
| Port allocation | None | PortAllocator tests |
| Instance CRUD | None | InstanceRepository tests |
| Subscription independence | None | Multi-instance subscription tests |
| Auto-start/recovery | None | Startup sequence tests |
| Core Adapter pattern | None | Adapter interface tests |
| Everything above | Tests exist for Instance/InstanceManager but have never been compiled or executed | Run in CTest (MI-121) |

---

## Upstream (Throne) Issues — informational only

CoreDeck no longer tracks upstream by rebase (DEC-011). Cherry-pick fixes deliberately and record them in DECISIONS.md.

| Throne Issue | Impact on CoreDeck | Status |
|--------------|-------------------|--------|
| sing-box version updates | Core compatibility | Watch releases |
| Xray API changes | XrayAdapter breaks | Watch releases |
| Qt6 version requirements | Build breaks | Test in CI |
| Database schema changes | Migration conflicts | Review before cherry-picking |

---

## Issue Template (for new issues)

```
## KI-XXX: Brief Title

**Severity:** High/Medium/Low
**Status:** OPEN/PLANNED/IN_PROGRESS/RESOLVED/WONT_FIX
**Component:** [Build|Core|UI|Database|Platform|Network]

### Description
Clear description of the issue.

### Reproduction Steps
1. Step one
2. Step two
3. ...

### Expected Behavior
What should happen.

### Actual Behavior
What actually happens.

### Environment
- OS:
- Qt version:
- Core version:
- Build type:

### Workaround
If any.

### Related
- Tasks: MI-XXX
- Decisions: DEC-XXX
```

---

*Update this file when issues are discovered, resolved, or re-prioritized.*