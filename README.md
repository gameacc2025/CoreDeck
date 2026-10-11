# CoreDeck

A multi-instance proxy client based on [Throne](https://github.com/throneproj/Throne), enabling a single UI to manage multiple independent proxy environments (Instances) simultaneously.

## Overview

CoreDeck extends Throne's architecture to support multiple independent **Instances** — each with its own:
- Core process (Xray, sing-box, or external)
- Local proxy ports (SOCKS, HTTP)
- Subscription groups and profiles
- Configuration and state
- Auto-start behavior

This replaces the need to run multiple separate GUI applications for different proxy configurations.

## Project Status

**Phase:** Phase 0b - Bootstrap (CI, Throne integration, first build)  
**Branch:** `feature/multi-instance`  
**Last Updated:** 2026-10-11

### Completed
- [x] Throne repository analyzed
- [x] Architecture assessment completed
- [x] Project documentation structure created (`project-docs/`)
- [x] **MI-118**: CI workflow provisioned (GitHub Actions: Windows MSVC, Linux GCC/Clang)
- [x] **MI-119**: Throne source code integrated into CoreDeck tree with root CMakeLists.txt
- [x] **MI-122**: Migration logic implemented (nullable groups.instance_id, backup, foreign_keys pragma)

### In Progress
- [ ] **MI-120**: First successful build and launch (CI verification running)
- [ ] **MI-121**: Build and run existing tests (MI-010..MI-020)
- [ ] **MI-123**: Remove temporary Throne folder and stale references

### Planned Phases
1. **Phase 0b:** Bootstrap — toolchain/CI, integrate Throne code, first build (M0, M1)
2. **Phase 1:** Instance data model & storage (MI-010..MI-016)
3. **Phase 2:** Default Instance & multi-Instance management (MI-020..MI-025)
4. **Phase 3:** Multiple independent Core processes (MI-030..MI-035)
5. **Phase 4:** Independent ports & Instance dashboard (MI-040..MI-045)
6. **Phase 5:** Independent subscriptions & profiles (MI-050..MI-054)
7. **Phase 6:** Startup, auto-start & state recovery (MI-060..MI-065)
8. **Phase 7:** Core Adapters (sing-box, External) — P1 (MI-070..MI-075)
9. **Phase 8:** Core Selection for Chain & Profiles — P1 (MI-080..MI-084)
10. **Phase 9:** Central Router — P2 (MI-090..MI-094)
11. **Phase 10:** TUN & Cross-Platform — P2 (Windows + Linux only) (MI-100..MI-104)
12. **Phase 11:** Testing, Optimization & Packaging (MI-110..MI-117)

## Quick Start

### Prerequisites
- Qt 6.6+
- CMake 3.20+
- C++20 compatible compiler (MSVC on Windows, GCC/Clang on Linux)
- Git
- Python 3 + aqtinstall (for Qt on Linux CI)

### Building CoreDeck

```bash
# Clone repository
git clone https://github.com/gameacc2025/CoreDeck.git
cd CoreDeck

# Windows (MSVC)
mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release

# Linux (GCC/Clang)
mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

### CI Build
Builds run automatically on GitHub Actions for Windows (MSVC) and Linux (GCC/Clang) on every push to `main` or `feature/*` branches. See [build.yml](.github/workflows/build.yml).

## Documentation

All project documentation is in [`project-docs/`](project-docs/):

| File | Description |
|------|-------------|
| [PROJECT_SPEC.md](project-docs/PROJECT_SPEC.md) | Complete product requirements and behavior specifications |
| [ARCHITECTURE.md](project-docs/ARCHITECTURE.md) | Actual and target architecture, components, interfaces |
| [ROADMAP.md](project-docs/ROADMAP.md) | Development phases, milestones, and long-term plans |
| [TASKS.md](project-docs/TASKS.md) | Prioritized task list with acceptance criteria |
| [PROGRESS.md](project-docs/PROGRESS.md) | Current status, latest verified progress, blockers |
| [CHANGELOG.md](project-docs/CHANGELOG.md) | User-visible and technical changes |
| [DECISIONS.md](project-docs/DECISIONS.md) | Architectural decisions, rationale, alternatives |
| [BUILD_AND_TEST.md](project-docs/BUILD_AND_TEST.md) | Verified build commands, test procedures, results |
| [KNOWN_ISSUES.md](project-docs/KNOWN_ISSUES.md) | Known bugs, limitations, risks, workarounds |
| [AI_WORKFLOW.md](project-docs/AI_WORKFLOW.md) | Mandatory rules for AI coding sessions |
| [DEPENDENCIES_AND_LICENSES.md](project-docs/DEPENDENCIES_AND_LICENSES.md) | Dependencies, versions, licenses, compliance |
| [MVP_SCOPE.md](project-docs/MVP_SCOPE.md) | Frozen P0/P1/P2 scope for prototype |

## Key References

- **Throne Repository:** https://github.com/throneproj/Throne
- **v2rayN Reference:** https://github.com/2dust/v2rayN (for Chain Core selection behavior)
- **CoreDeck Repository:** https://github.com/gameacc2025/CoreDeck

## License

CoreDeck is a fork of Throne, which is licensed under **GPL-3.0**. All modifications and additions maintain this license.

See [Throne's LICENSE](Throne/LICENSE) and [DEPENDENCIES_AND_LICENSES.md](project-docs/DEPENDENCIES_AND_LICENSES.md) for third-party license obligations.