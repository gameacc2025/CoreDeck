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

**Phase:** Phase 0 - Repository Analysis & Baseline Establishment  
**Branch:** `feature/multi-instance`  
**Last Updated:** 2026-10-10

### Completed
- [x] Throne repository cloned and analyzed
- [x] Architecture assessment completed
- [x] Project documentation structure created

### In Progress
- [ ] Project specification documentation
- [ ] Architecture documentation
- [ ] Roadmap and task breakdown
- [ ] Build verification

### Planned Phases
1. **Phase 0:** Repository analysis, baseline build, documentation setup
2. **Phase 1:** Instance data model and storage design
3. **Phase 2:** Default Instance and multi-Instance management
4. **Phase 3:** Multiple independent Xray processes
5. **Phase 4:** Independent ports and Instance dashboard
6. **Phase 5:** Independent subscriptions and profiles
7. **Phase 6:** Startup, auto-start, and state recovery
8. **Phase 7:** Core adapters (sing-box, external cores)
9. **Phase 8:** Core selection for Chain and profiles
10. **Phase 9:** Central Router
11. **Phase 10:** TUN and cross-platform integration
12. **Phase 11:** Testing, optimization, packaging

## Quick Start

### Prerequisites
- Qt 6.x
- CMake 3.20+
- C++20 compatible compiler (MSVC, GCC, Clang)
- Git

### Building Throne (Baseline)
```bash
cd Throne
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### Building CoreDeck
```bash
# After Phase 1+ implementation
cd CoreDeck
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

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

## Key References

- **Throne Repository:** https://github.com/throneproj/Throne
- **v2rayN Reference:** https://github.com/2dust/v2rayN (for Chain Core selection behavior)
- **NekoRay (Historical):** https://github.com/MatsuriDayo/nekoray
- **CoreDeck Repository:** https://github.com/DevastBit/CoreDeck

## License

CoreDeck is a fork of Throne, which is licensed under **GPL-3.0**. All modifications and additions maintain this license.

See [Throne's LICENSE](Throne/LICENSE) and [DEPENDENCIES_AND_LICENSES.md](project-docs/DEPENDENCIES_AND_LICENSES.md) for third-party license obligations.