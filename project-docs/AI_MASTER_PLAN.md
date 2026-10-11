# CoreDeck AI Master Execution Plan

> **Purpose:** Autonomous execution contract for an AI coding agent working on CoreDeck. Read it at the start of every session.
>
> **Repository:** `https://github.com/gameacc2025/CoreDeck.git`
>
> **Working directory (current Windows setup):** `G:\Project\CoreDeck\`
>
> **Targets:** Windows 10/11 (primary) and Linux. macOS is out of scope (DEC-012).
>
> **Throne status:** CoreDeck is a fork whose future development is owned entirely by the project owner. Throne code is **absorbed into CoreDeck's own source tree** (DEC-011). A local `Throne/` folder is only a temporary source for copying files and is deleted once integration is verified.

---

## 0. Operating Mode: Fully Autonomous (DEC-013)

The project owner wants the AI to carry the project to a **testable prototype with no human involvement**.

- **Do not ask for approval** for ordinary decisions: task selection, commits, pushes, tool installation, dependency choices within the license policy, scope trade-offs, refactors required by a task.
- **Commit and push automatically** after every verified stage on the working branch. Do not wait for permission.
- **Do not end a session after one task.** Keep looping through Section 5 until the Prototype Definition (Section 11) is met or a genuine blocker (Section 10) occurs.
- **Do not ask clarifying questions** when a reasonable, documented default exists. Choose it, record it in `DECISIONS.md`, and continue.
- Install missing development tools yourself (Section 7). Only stop if installation truly needs something the AI cannot do (e.g. interactive admin consent that was refused, paid license).
- If an execution limit interrupts the session, leave the repo committed, pushed and documented so the next session resumes without guessing.

The owner reviews results at the end. Honesty is therefore mandatory: never claim a build, test, commit, push or remote state you did not observe.

---

## 1. Mission

Turn the existing Throne-based code into **CoreDeck**: a maintainable Windows/Linux desktop proxy client that manages multiple independent proxy Instances from one GUI (one GUI process, one Core process per Instance).

Do not stop at plans or foundations. Deliver a **prototype that builds, launches, and runs the essential proxy workflow end to end**, then keep improving.

### Session start
1. Read this file, `project-docs/AI_WORKFLOW.md`, and `PROJECT_SPEC.md`, `ARCHITECTURE.md`, `DECISIONS.md`, `ROADMAP.md`, `TASKS.md`, `PROGRESS.md`, `BUILD_AND_TEST.md`, `KNOWN_ISSUES.md`, `DEPENDENCIES_AND_LICENSES.md`, `MVP_SCOPE.md`.
2. Read the latest report in `project-docs/sessions/`.
3. Inspect the real tree, `git status`, branch, remotes, recent commits.
4. Trust verifiable repository state over stale docs; fix the docs.
5. Pick the first unfinished task whose dependencies are met and proceed.

---

## 2. Source of truth and conflict resolution

Priority order:
1. Explicit owner instructions in the current conversation.
2. `project-docs/DECISIONS.md` and approved spec/architecture.
3. This master plan.
4. `TASKS.md` (the single source of task IDs) and `ROADMAP.md`.
5. Existing code, tests, older session reports.

`TASKS.md` owns task IDs. If `ROADMAP.md` disagrees, `TASKS.md` wins and the roadmap is corrected.

If two authoritative sources conflict, **resolve it yourself** using this order, record the resolution in `DECISIONS.md`, and continue. Escalate only when Section 10 applies.

---

## 3. Repository and Git rules

### Canonical remote
`origin` must be `https://github.com/gameacc2025/CoreDeck.git`. Verify at session start; correct if wrong. Keep historical reports accurate.

If the remote has commits not in local history, fetch and integrate with a normal merge or rebase of your own unpushed commits. Never force-push, `reset --hard` shared history, or rewrite published history.

### Automatic Git discipline
- Check `git status --short` and the diff before every commit.
- Preserve pre-existing owner changes.
- Stage explicit paths only (no blind `git add -A`), and never stage build output, caches, secrets, or temporary Throne reference folders.
- Small, scoped commits with conventional messages (`feat:`, `fix:`, `test:`, `docs:`, `refactor:`, `build:`, `ci:`).
- After each verified stage: update docs, commit, **push automatically**, verify the push (exit status plus `git ls-remote origin <branch>` matching local HEAD).
- Work on `feature/multi-instance`. Merge into `main` automatically only when the prototype gate (Section 11) passes or when a milestone (M1, M2, M3) is verified, using a normal (non-force) push.
- No empty commits. No force-push. No secrets in anything committed.
- If push fails (auth/permission/network), keep the local commit, record the exact error, retry later, and continue only with safe local work. Do not claim a backup exists.
- Do not create tokens or change account security. Use the already-configured credential mechanism.

---

## 4. Throne is part of CoreDeck (DEC-011)

CoreDeck is a self-contained repository. Throne is **not** a permanent reference, sibling, submodule, or runtime dependency. The owner will develop CoreDeck independently from now on.

### Integration procedure
1. Inspect the Throne checkout (`Throne/`, temporary): structure, CMake, `3rdparty/`, resources, translations, `.ui` files, scripts, license headers.
2. Decide what CoreDeck needs. Integrate **all code required for the MVP** (core app, database, config generation, process/RPC, UI, platform layers for Windows and Linux, required `3rdparty` libraries, resources, translations, build scripts). Skip macOS-only code, unrelated examples, build outputs, caches, and upstream CI that does not apply.
3. Place files in CoreDeck's own tree following the repository's conventions (`src/`, `include/`, `res/`, `3rdparty/`, `cmake/`, `tests/`, etc.), keeping upstream layout where that avoids needless churn.
4. Preserve upstream copyright/license headers and `LICENSE` (GPL-3.0). Add attribution for Throne and its upstream authors in `README.md` and `DEPENDENCIES_AND_LICENSES.md`.
5. Create CoreDeck's own root `CMakeLists.txt`. Rename binary, window title, app-data paths, and identifiers to CoreDeck only where the spec requires, and keep Throne-DB backward compatibility (DEC-006).
6. Build and test using **only** the CoreDeck repo plus documented external dependencies.
7. Search all tracked files for `Throne/`, `G:\Project\CoreDeck\Throne`, and sibling-path references; remove build/runtime dependence on them.
8. Once the integrated tree builds and tests pass, delete the temporary `Throne/` folder **only after** confirming its exact path is `<repo root>/Throne`, that it is not tracked by CoreDeck, and that it holds no owner-made work (check `git -C Throne status` and its remote/branch). Never delete on a guessed path.
9. Keep `Throne/` out of commits while it exists (use `.git/info/exclude`, which is local and does not alter the repo).
10. Record the integration in `PROGRESS.md`, `CHANGELOG.md`, `DEPENDENCIES_AND_LICENSES.md`, and a session report. Commit and push each logical integration step separately.

### Safety
- Do not blindly copy the whole upstream repository.
- Do not delete `Throne/` before every needed file is integrated and the build passes.
- If a license or missing-source problem blocks a component, document it, choose the safest alternative, and continue.

---

## 5. Autonomous task loop

**A. Select** the earliest eligible task by priority (P0 first) and dependency order. Don't skip ahead for convenience.

**B. Define** acceptance criteria, files likely to change, tests, and risks before coding.

**C. Implement** real behavior with error handling and cleanup. No placeholders, fake success paths, or test-only hacks in production code. Preserve data compatibility.

**D. Verify.** Build, run targeted tests, then broader tests. Report **passed / failed / blocked / not run** exactly. Fix failures caused by your change. Never weaken tests to pass.

**E. Document.** Update `TASKS.md`, `PROGRESS.md`, `ROADMAP.md`, `BUILD_AND_TEST.md`, `KNOWN_ISSUES.md`, `CHANGELOG.md`, `DEPENDENCIES_AND_LICENSES.md` as relevant, and add a session report in `project-docs/sessions/`.

**F. Commit, push, verify, continue** (Section 3). Then start the next task without waiting.

### Task status vocabulary
`TODO` → `IN_PROGRESS` → `DONE_UNVERIFIED` (code written, not built or tests not run) → `VERIFIED` (built and relevant tests actually passed). `BLOCKED` with a stated reason. Never mark `VERIFIED` on static review.

---

## 6. Build the application end to end

Connect the pieces as you go: UI → services → repositories/adapters; schema, migrations, models agree; adapters share a contract and report failures clearly; PortAllocator checks real bind availability; process start/monitor/stop/restart/cleanup work; import/export and migration protect data; settings persist and validate; logs never leak secrets; UI reflects real backend state; packaging for Windows and Linux is documented.

Do not call the application complete until it builds and meaningful acceptance checks actually ran.

---

## 7. Environment and toolchain (self-provisioned)

Build and run early. M0 comes before any new feature.

At session start, detect: git, CMake, Ninja, compiler (MSVC Build Tools or MinGW-w64; GCC/Clang on Linux), Qt 6.5+ (6.6+ preferred), Python.

If something is missing, **install it yourself** with supported, non-interactive methods, then retry:
- Windows: `winget install` (e.g. `Kitware.CMake`, `Ninja-build.Ninja`, `Microsoft.VisualStudio.2022.BuildTools` with the C++ workload, `Python.Python.3.12`); Qt via `aqtinstall` (`pip install aqtinstall`, then `aqt install-qt windows desktop 6.6.x win64_msvc2019_64` or the MinGW arch) into a documented folder such as `C:\Qt`.
- Linux: package manager (`apt`, `dnf`, `pacman`) for `cmake ninja-build g++ qt6-base-dev qt6-tools-dev libqt6dbus6` etc.
- Record exact commands and results in `BUILD_AND_TEST.md`.

If local installation is impossible (permission, disk, network), **fall back to GitHub Actions**: add `.github/workflows/build.yml` building and testing on `windows-latest` and `ubuntu-latest`, push, and read the run results (via `gh` or the API with the existing credential) as verification evidence. A green CI run on the pushed commit counts as "built and tested", and the log must be cited.

Never leave the same toolchain blocker unresolved across sessions.

---

## 8. Database and migration safety

Review the existing schema before changing it. Use explicit schema versions, transaction-wrapped deterministic migrations, a backup of the DB file before migrating (copy to `*.pre-coredeck.bak`), and idempotent steps. Enable `PRAGMA foreign_keys = ON` on every connection. Never silently discard unknown fields. Test fresh DBs and upgrades from real Throne-format fixtures using temporary databases only; never touch a real user DB in tests.

SQLite note: `ALTER TABLE ADD COLUMN` with a `REFERENCES` clause requires a `NULL` default. Add `groups.instance_id` as nullable, backfill it with the Default Instance UUID in the same transaction, then enforce the relationship in code and with indexes (or rebuild the table if strict NOT NULL is required).

---

## 9. Security, quality gates, testing

- Treat imports, subscriptions, configs, and process output as untrusted. Validate paths, avoid shell-string construction, use argument lists, never log secrets, bind to `127.0.0.1` by default.
- **Unit tests are mandatory** for every new or changed class, validation rule, migration, and bug fix, covering normal, boundary, invalid and error paths. Add integration tests across component/DB/process boundaries. Prioritize risk: validation, UUID identity, persistence, migration, port allocation, process lifecycle, proxy activation, secrets.
- Tests must assert real behavior. No empty or always-pass tests.
- Wire tests into CMake/CTest so `ctest --output-on-failure` runs them.
- Report per stage: test files, behaviors covered, exact commands, pass/fail counts, build status, what was not run and why.

---

## 10. When to stop (genuine blockers only)

Stop only the affected work, record it in `KNOWN_ISSUES.md` and `PROGRESS.md`, continue independent work, and report if:
- Credentials or permissions prevent a required operation after retries.
- The remote has divergent history that cannot be merged without discarding someone's work.
- A license problem makes shipping a required component unlawful.
- A required resource is unavailable and no safe alternative or CI fallback exists.
- The next action would be destructive, require force-push, or expose secrets.

Report: the blocked task and step, what you inspected and ran, the exact error, what is safely done, the minimum action needed from the owner, and what can continue meanwhile.

Everything else is decided by the AI under Section 0.

---

## 11. Prototype definition and MVP policy

### MVP-first
Deliver a working, installable, testable first version as fast as reasonably possible. No architectural perfection or speculative features before a usable app exists. Reuse and adapt Throne code rather than rewriting working components. Keep `project-docs/MVP_SCOPE.md` current and the P0 list frozen unless a change is documented.

- **P0 (prototype/MVP):** builds and launches on Windows (and Linux build verified in CI); main window and Instance/profile management; stable UUID Instances; import of supported profile/subscription formats; Xray core located/obtained and started, monitored, stopped; clear errors for missing binary, invalid config, port conflict; per-Instance SOCKS/HTTP local proxy; PortAllocator with real bind checks; settings persistence; in-place Throne data migration; clean child-process shutdown; logging without secrets; critical tests; documented build and packaging; smoke test.
- **P1:** sing-box and external cores, advanced lifecycle, richer diagnostics, UI polish, broader regression tests.
- **P2:** Central Router, TUN per Instance, optional integrations, performance work without a measured problem.

Do not build P1/P2 while P0 is broken, unless it is a prerequisite.

### Milestones
- **M0** toolchain ready (or CI building) and first configure/build result recorded.
- **M1** Throne code integrated, CoreDeck builds and launches, no dependence on a Throne folder, initial tests run.
- **M2** basic proxy workflow works: create Instance/profile, start/stop core, local proxy works, port conflicts handled, state persists.
- **M3** prototype ready: all P0 done, critical tests pass, smoke test passes, Windows package produced, Linux build verified, limitations documented.

### Prototype gate (when the AI may declare "ready for owner testing")
1. Builds from a clean checkout of CoreDeck alone, with documented steps (local or CI).
2. Launches; Default Instance appears; create/edit/delete Instances works.
3. Two Instances can run simultaneously with different ports and PIDs; stopping one leaves the other running.
4. Port conflict, missing core binary, and invalid config show actionable errors without crashing the GUI.
5. Settings and Instances survive restart; migration from a Throne-format DB is tested on fixtures.
6. `ctest` passes with recorded counts; smoke test result recorded.
7. A Windows portable ZIP (and Linux build artifact where feasible) exists and is described in `README.md`.
8. Everything is committed, pushed, and the remote head verified; `Throne/` folder removed or explicitly noted as pending.
9. A final report lists verified behavior versus known limitations and exact steps for the owner to test.

If any item is unmet, call it a *prototype candidate*, not ready, and keep working.

---

## 12. Required report per stage

- **Task/stage:** ID and title
- **Result:** VERIFIED / DONE_UNVERIFIED / BLOCKED / FAILED
- **Implementation:** key changes and files
- **Verification:** commands and actual outcomes
- **Documentation:** files updated
- **Git:** commit hash, branch, push result, remote verification
- **Next task**

Then continue automatically to the next eligible task.

---

## 13. First-run checklist

1. Inspect repo and Git state; verify `origin`.
2. Exclude `Throne/` locally via `.git/info/exclude` if it is untracked.
3. Read all project docs and the latest session report.
4. Re-check the toolchain and install what is missing (Section 7) or set up CI. Reach **M0**.
5. Integrate required Throne code (Section 4) and create the root CMake build. Reach **M1**.
6. Reclassify earlier task statuses honestly: MI-010 through MI-020 are `DONE_UNVERIFIED` until built and tested. Build and run their tests, fix failures, then mark `VERIFIED`.
7. Correct the SQLite `groups.instance_id` approach and `PRAGMA foreign_keys` handling (Section 8) and add migration tests with Throne-format fixtures.
8. Continue down `TASKS.md` by P0 priority until the prototype gate passes.

**The desired behavior is sustained autonomous progress in small, reviewable, tested, documented, committed and pushed stages, without waiting for the owner.**
