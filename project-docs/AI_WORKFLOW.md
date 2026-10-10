# CoreDeck AI Development Workflow

**Version:** 1.0  
**Status:** MANDATORY  
**Last Updated:** 2026-10-10

---

## Purpose

This document defines **mandatory rules** for all AI coding sessions working on CoreDeck. It ensures continuity, quality, and safety across sessions and agents.

**Non-negotiable:** Every session must follow these rules. Deviations require explicit user approval.

---

## 1. Session Start Protocol

**Before writing any code, you MUST:**

1. **Read these files in order:**
   - `README.md` (project root)
   - `project-docs/AI_WORKFLOW.md` (this file)
   - `project-docs/PROGRESS.md`
   - `project-docs/TASKS.md`
   - `project-docs/ROADMAP.md`
   - `project-docs/KNOWN_ISSUES.md`

2. **Inspect repository state:**
   ```bash
   git status
   git log --oneline -10
   git branch --show-current
   ```

3. **Review relevant architecture/docs:**
   - `project-docs/ARCHITECTURE.md` for component design
   - `project-docs/PROJECT_SPEC.md` for requirements
   - `project-docs/DECISIONS.md` for prior choices

4. **Identify next task:**
   - Check `TASKS.md` for highest priority `TODO`/`IN_PROGRESS`
   - Verify dependencies are met
   - Confirm no `BLOCKED` status without resolution

5. **Announce plan:**
   - State which task(s) you'll work on
   - Reference task IDs (e.g., "Starting MI-010: Instance C++ class design")

---

## 2. Development Rules

### Code Changes
- **Read before write:** Never edit a file without reading it first
- **Minimal changes:** Touch only what's necessary for the task
- **Preserve existing code:** Don't reformat, rename, or refactor without reason
- **Follow existing patterns:** Match Throne's code style, naming, error handling
- **No dead code:** Don't leave commented-out code or unused imports

### Architecture Compliance
- **Respect DECISIONS.md:** All architectural decisions are binding
- **Core Adapter pattern:** New Core support via `CoreAdapter` interface (DEC-003)
- **Instance isolation:** Each Instance = separate Core process (DEC-001)
- **UUID primary keys:** Use `QUuid` for Instance identifiers (DEC-002)
- **PortAllocator:** All port management through central service (DEC-004)

### Database Changes
- **Migration-first:** Schema changes require migration plan in `DECISIONS.md`
- **Backward compatibility:** Throne must still read migrated DB
- **Foreign keys:** Use `ON DELETE CASCADE` for Instance-owned data
- **No raw SQL in UI:** Use Repository classes

### UI Changes
- **Qt Designer .ui files:** Prefer for new dialogs/widgets
- **Signal/Slot:** Use Qt's mechanism, not manual callbacks
- **Model/View:** Use `QAbstractItemModel` for lists
- **No blocking calls:** Long operations on worker threads

---

## 3. Task Management

### Task Lifecycle
```
TODO → IN_PROGRESS → DONE → VERIFIED
              ↘ BLOCKED (if dependency missing)
```

### Updating TASKS.md
**After each task:**
1. Change status to `DONE` (implementation complete)
2. Fill in:
   - Test method used
   - Actual test result
   - Any blockers or notes
3. If tests pass → `VERIFIED`
4. If blocked → `BLOCKED` with reason

### Creating New Tasks
- Add to `TASKS.md` in appropriate phase
- Assign unique ID: `MI-XXX` (sequential)
- Include all fields: acceptance criteria, test method, dependencies
- Get user approval before starting unplanned work

---

## 4. Testing Requirements

### Mandatory Testing
- **Unit tests** for new classes (Repository, Adapter, Manager)
- **Integration tests** for multi-component flows
- **Manual verification** for UI changes
- **Build verification** after any CMake/source change

### Test Documentation
Record in `TASKS.md` and `BUILD_AND_TEST.md`:
- Exact commands run
- Actual output (success/failure)
- Screenshots for UI (if applicable)
- Performance numbers for benchmarks

### No False Claims
- **Never claim** a test passed if not actually run
- **Never claim** build succeeded without output evidence
- **Report failures honestly** — they're valuable data

---

## 5. Documentation Updates

**After every meaningful task, update:**

| File | When |
|------|------|
| `TASKS.md` | Task status, test results, notes |
| `PROGRESS.md` | Current status, blockers, next steps |
| `CHANGELOG.md` | User-visible changes |
| `DECISIONS.md` | New architectural decisions |
| `BUILD_AND_TEST.md` | Verified commands, results |
| `KNOWN_ISSUES.md` | New issues discovered |
| `ARCHITECTURE.md` | If architecture changed |

### Session Report
**At session end, create:**
`project-docs/sessions/YYYY-MM-DD-session-NNN.md`

Include:
- Session goal
- Tasks completed (with IDs)
- Files changed
- Decisions made
- Tests run + results
- Blockers remaining
- Exact next steps for next session

---

## 6. Git & Repository Safety

### Commits
- **No auto-commit:** Only commit when user explicitly requests
- **Atomic commits:** One logical change per commit
- **Conventional messages:** `feat:`, `fix:`, `refactor:`, `docs:`, `test:`
- **No secrets:** Never commit keys, tokens, passwords, subscription URLs

### Branches
- Work on `feature/multi-instance` or task-specific branches
- `main` = stable, `dev` = integration (future)
- Never force-push without approval

### Upstream (Throne)
- Throne in `Throne/` is **read-only reference**
- Don't modify Throne directly
- Rebase CoreDeck on Throne updates only with approval

---

## 7. Safety & Compliance

### Licenses
- **GPL-3.0:** CoreDeck inherits from Throne
- **Track all dependencies:** `DEPENDENCIES_AND_LICENSES.md`
- **No proprietary code** without license verification
- **Third-party code:** Document source, license, version

### Data Privacy
- **Never log** subscription URLs, tokens, passwords
- **Never commit** real user data or configs
- **Use placeholders** in examples: `YOUR_SUBSCRIPTION_URL`
- **Local DB:** SQLite unencrypted (assume disk encryption)

### Network
- **Default bind:** `127.0.0.1` only
- **No `0.0.0.0`** without explicit user action + warning
- **Validate all inputs:** Paths, URLs, ports, CLI args

---

## 8. Communication Protocol

### With User
- **Concise updates:** What done, what next, blockers
- **Ask before assuming:** Clarify ambiguity before implementing
- **Show evidence:** Test output, build logs, screenshots
- **Admit uncertainty:** "I need to verify X before proceeding"

### With Other AIs (via Board/Session Reports)
- **Document decisions** in `DECISIONS.md` and session reports
- **Flag blockers** in `PROGRESS.md` and `KNOWN_ISSUES.md`
- **Handoff quality:** Next AI should need zero context from chat

---

## 9. Prohibited Actions

| Action | Reason |
|--------|--------|
| Delete/modify Throne/ upstream | Read-only reference |
| Commit without user approval | Safety, review |
| Push without approval | Remote safety |
| Hardcode paths/ports | Portability |
| Skip tests | Quality |
| Claim unverified results | Integrity |
| Store secrets in code/docs | Security |
| Reformat unrelated code | Noise, risk |
| Add dependencies without docs | Compliance |

---

## 10. Emergency Procedures

### Build Broken
1. Revert last change (`git checkout -- .`)
2. Verify baseline builds
3. Re-apply incrementally with tests

### Data Corruption
1. Stop all Core processes
2. Backup `throne.db`
3. Run migration repair (if implemented)
4. Report in `KNOWN_ISSUES.md`

### Upstream Conflict
1. Document in `DECISIONS.md`
2. Create `BLOCKED` task
3. Await user direction

---

## 11. Quick Reference Checklist

**Session Start:**
- [ ] Read all 6 core docs
- [ ] `git status` clean
- [ ] Next task identified
- [ ] Plan announced

**During Work:**
- [ ] Read file before edit
- [ ] Follow existing patterns
- [ ] Update TASKS.md per task
- [ ] Run tests after changes
- [ ] Document results honestly

**Session End:**
- [ ] All docs updated
- [ ] Session report written
- [ ] PROGRESS.md current
- [ ] Next task clear
- [ ] No uncommitted secrets

---

## 12. Enforcement

This workflow is **binding**. Violations:
- First: Warning in session report
- Repeated: Task reverted, user notified
- Critical (secrets, data loss): Immediate stop, user alerted

**User has final authority** on all decisions not covered here.

---

*This workflow enables reliable multi-session, multi-agent development. Follow it rigorously.*