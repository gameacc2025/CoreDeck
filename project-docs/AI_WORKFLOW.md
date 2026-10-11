# CoreDeck AI Development Workflow

**Version:** 2.0  
**Status:** MANDATORY  
**Last Updated:** 2026-10-10  
**Supersedes:** v1.0 (which required user approval for every commit/push; replaced by DEC-013)

---

## Purpose

Mandatory rules for every AI coding session on CoreDeck. They guarantee continuity, quality and safety across sessions and agents.

**Operating mode (DEC-013): fully autonomous.** The owner does not want to be involved until a testable prototype exists. The AI decides, builds, tests, commits and pushes by itself. `AI_MASTER_PLAN.md` is the execution contract; this file holds the day-to-day rules. If they ever disagree, `AI_MASTER_PLAN.md` wins.

---

## 1. Session Start Protocol

Before writing code:

1. Read, in order: `README.md`, `project-docs/AI_MASTER_PLAN.md`, this file, `PROGRESS.md`, `TASKS.md`, `ROADMAP.md`, `KNOWN_ISSUES.md`, `MVP_SCOPE.md`, and the newest file in `project-docs/sessions/`.
2. Inspect the repository:
   ```bash
   git status --short
   git log --oneline -10
   git branch --show-current
   git remote -v
   ```
3. Read `ARCHITECTURE.md`, `PROJECT_SPEC.md`, `DECISIONS.md` as needed.
4. Check the toolchain (CMake, Ninja, compiler, Qt, Python). Install what is missing or use CI (Master Plan section 7).
5. Pick the next eligible task: highest priority (P0 first), dependencies met, not `BLOCKED`.
6. State the plan in one or two lines with task IDs, then start. Do not wait for a reply.

---

## 2. Development Rules

### Code
- Read a file before editing it.
- Make the smallest complete change that satisfies the task.
- Follow the existing code style and patterns of the (now integrated) Throne code.
- No dead code, commented-out code, or unused imports.
- No placeholders, fake success paths, or test-only behavior in production code.

### Architecture (see DECISIONS.md, all binding)
- One GUI process, one Core process per Instance (DEC-001, DEC-008).
- Instance primary key is a UUID (`QUuid`) (DEC-002).
- Core support goes through the `CoreAdapter` interface (DEC-003).
- All ports go through `PortAllocator` (DEC-004).
- Local Proxy Mode first; Central Router later (DEC-005).
- Throne code is part of CoreDeck's own tree (DEC-011). Never depend on a `Throne/` folder.
- Targets are Windows and Linux only (DEC-012).

### Database
- Schema changes need an explicit, versioned, transaction-wrapped migration.
- Back up the DB file before migrating (`*.pre-coredeck.bak`).
- Enable `PRAGMA foreign_keys = ON` on every connection.
- SQLite `ALTER TABLE ... ADD COLUMN ... REFERENCES` needs a `NULL` default: add nullable, backfill, enforce in code and via index.
- Keep the migrated DB readable by Throne where practical; verify it, don't assume it.
- No raw SQL in UI code; use repositories.

### UI
- Qt Designer `.ui` files for new dialogs/widgets where practical.
- Signals/slots, `QAbstractItemModel` for lists, no blocking calls on the GUI thread.

---

## 3. Task Management

### Lifecycle
```
TODO -> IN_PROGRESS -> DONE_UNVERIFIED -> VERIFIED
                 \-> BLOCKED (reason required)
```
- `DONE_UNVERIFIED`: code written, but it was not built, or its tests were not executed.
- `VERIFIED`: built, and the relevant tests were actually executed and passed (local run or cited CI run).
- Static review never counts as verification.

### TASKS.md
`TASKS.md` is the single source of task IDs. After each task record status, exact test command, actual result, notes. Add new tasks with the next sequential `MI-XXX` ID and all fields; the AI may add tasks needed for the MVP without asking.

---

## 4. Testing

- Unit tests are mandatory for every new/changed class, validation rule, migration and bug fix: normal, boundary, invalid and error paths.
- Integration tests when behavior crosses component, DB, process or UI/service boundaries.
- Tests run through CTest: `ctest --output-on-failure`.
- Use temporary databases and fixtures; never touch real user data.
- Build verification after any CMake or source change.
- Record exact commands and actual output in `BUILD_AND_TEST.md` and `TASKS.md`.

### No false claims
Never claim a build, test, commit, push, or remote state that was not observed. Report failures honestly.

---

## 5. Documentation

After every meaningful stage update what applies:

| File | When |
|------|------|
| `TASKS.md` | status, test results |
| `PROGRESS.md` | status, blockers, next task, milestone, latest commit |
| `CHANGELOG.md` | user-visible changes |
| `DECISIONS.md` | new decisions (record them yourself, do not ask) |
| `BUILD_AND_TEST.md` | verified commands and results |
| `KNOWN_ISSUES.md` | issues found/resolved |
| `ARCHITECTURE.md` | architecture changes |
| `DEPENDENCIES_AND_LICENSES.md` | dependency or licence changes |
| `MVP_SCOPE.md` | P0/P1/P2 changes |

Session report at the end of each session: `project-docs/sessions/YYYY-MM-DD-session-NNN.md` with goal, tasks (IDs), files changed, decisions, tests run and results, blockers, exact next steps.

---

## 6. Git (automatic)

### Commits and pushes: automatic
- After each verified stage the AI commits and pushes **without asking**.
- Atomic commits, conventional messages: `feat:`, `fix:`, `refactor:`, `docs:`, `test:`, `build:`, `ci:`.
- Stage explicit paths only; no blind `git add -A`.
- Check `git status --short` and the diff before committing. No secrets, tokens, subscription URLs, user data, build output, or caches.
- Verify every push: exit status and `git ls-remote origin <branch>` equals local HEAD.
- If a push fails, keep the local commit, record the exact error, retry, continue with safe local work, and never claim remote backup.

### Branches
- Day-to-day work on `feature/multi-instance`.
- Merge to `main` automatically (normal, non-force push) when milestone M1, M2 or M3 is verified, or when the prototype gate passes.
- `dev` is not used until needed.

### Never, even autonomously
- Force-push, `reset --hard` on shared history, history rewriting.
- Committing secrets or real user data.
- Changing GitHub account security settings or creating tokens.

### Throne source
- A temporary `Throne/` folder is only a source to copy from. Keep it out of commits (`.git/info/exclude`).
- Integrate what CoreDeck needs into CoreDeck's own tree (Master Plan section 4), then delete the folder after verifying its exact path and that it holds no owner work.
- CoreDeck is developed independently from now on; there is no ongoing upstream rebase obligation. Upstream fixes may be cherry-picked deliberately and recorded in `DECISIONS.md`.

---

## 7. Safety & Compliance

### Licences
- CoreDeck is GPL-3.0-or-later (derived from Throne). Preserve upstream licence headers and attribution.
- Track every dependency in `DEPENDENCIES_AND_LICENSES.md` and verify each licence against official sources before shipping.
- No proprietary code. Document third-party source, licence and version.

### Data privacy
- Never log subscription URLs, tokens, passwords, private keys.
- Never commit real configs or user data; use placeholders like `YOUR_SUBSCRIPTION_URL`.

### Network
- Bind to `127.0.0.1` by default; `0.0.0.0` only by explicit user action with a warning.
- Validate paths, URLs, ports, CLI arguments. Use argument lists, not shell strings.

---

## 8. Autonomy and Communication

- Decide ordinary matters yourself; record significant ones in `DECISIONS.md`.
- Ask the owner only for a genuine blocker (Master Plan section 10).
- Updates are concise: what was done, evidence, what is next.
- Handoff quality: the next session must need zero chat context. Everything lives in the repo docs.

---

## 9. Prohibited Actions

| Action | Reason |
|--------|--------|
| Depend on a `Throne/` folder or absolute external path | Self-contained repo (DEC-011) |
| Delete a directory based on a guessed path | Data loss |
| Force-push / rewrite published history | Remote safety |
| Commit secrets or real user data | Security |
| Claim unverified build/test/push results | Integrity |
| Mark `VERIFIED` without an executed test | Integrity |
| Weaken or delete tests to get green | Quality |
| Hardcode paths/ports | Portability |
| Add dependencies without documenting licence | Compliance |
| Build P1/P2 features while P0 is broken | Scope control |
| Add macOS-specific work | Out of scope (DEC-012) |

---

## 10. Emergency Procedures

### Build broken
1. Identify the commit that broke it (`git log`, `git bisect` if needed).
2. Fix forward; if not quickly fixable, `git revert` that commit (a new commit, no history rewrite).
3. Re-verify the baseline, then re-apply incrementally with tests.

### Data corruption
1. Stop all Core processes.
2. Restore from `*.pre-coredeck.bak`.
3. Fix the migration, add a regression test, record in `KNOWN_ISSUES.md`.

### Remote divergence
1. `git fetch`; inspect the remote-only commits.
2. Merge or rebase your own unpushed work on top (never discard theirs).
3. If impossible without losing work, record a blocker and continue locally.

---

## 11. Quick Checklist

**Session start:** docs read, `git status` understood, toolchain checked, next task picked.  
**During work:** read before edit, tests with code, build after changes, docs updated.  
**Stage end:** tests run, docs updated, commit, push, remote verified, next task started.  
**Session end:** session report written, `PROGRESS.md` current, repo pushed.

---

## 12. Enforcement

Violations of the integrity rules (false claims, secrets, destructive git operations) must be recorded in the session report and corrected immediately. The owner's explicit instructions in the current conversation always take precedence.

---

*Revision history: v1.0 (2026-10-10) initial; v2.0 (2026-10-10) autonomous commit/push, integrated Throne, Windows+Linux only, DONE_UNVERIFIED status.*
