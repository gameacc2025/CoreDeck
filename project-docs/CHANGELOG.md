# CoreDeck Changelog

**Version:** 0.1 (Development)  
**Format:** Based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)

---

## [Unreleased] - Development

### Added
- Project documentation structure (`project-docs/`)
- `README.md` - Project overview, quick start, documentation index
- `PROJECT_SPEC.md` - Complete product specification with 17 sections
- `ARCHITECTURE.md` - Current Throne architecture analysis + target CoreDeck architecture
- `ROADMAP.md` - 11-phase development plan with detailed tasks
- `TASKS.md` - 117 tasks with acceptance criteria and test methods
- `PROGRESS.md` - Current status tracker
- CoreDeck Git repository initialized at `G:\Project\CoreDeck\`
- Throne upstream cloned for reference at `G:\Project\CoreDeck\Throne\`

### Analyzed
- Throne architecture: Qt6, C++20, CMake, SQLite, sing-box/Xray
- Core execution flow: Profile → Config Generation → RPC → CoreProcess
- Data model: Settings (KV), Groups, Profiles, Routes, IpLists
- Single-instance enforcement via QLocalServer
- RPC protocol: Protobuf over local sockets
- Build system: CMake with platform-specific configs

---

## [0.0.0] - 2026-10-10 (Project Initialization)

### Project Setup
- CoreDeck repository created as fork of Throne
- Documentation framework established
- Development workflow defined (AI_WORKFLOW.md pending)
- License compliance: GPL-3.0 inherited from Throne

---

## Release Template (for future versions)

```
## [X.Y.Z] - YYYY-MM-DD

### Added
- New features

### Changed
- Changes in existing functionality

### Deprecated
- Soon-to-be removed features

### Removed
- Now removed features

### Fixed
- Bug fixes

### Security
- Vulnerability fixes
```

---

## Categories

| Category | Description |
|----------|-------------|
| **Added** | New features, capabilities, UI elements |
| **Changed** | Modifications to existing behavior |
| **Deprecated** | Features marked for future removal |
| **Removed** | Features removed in this version |
| **Fixed** | Bug fixes |
| **Security** | Security-related fixes |
| **Performance** | Performance improvements |
| **Documentation** | Documentation updates |

---

## Versioning Scheme

- **Major (X):** Breaking changes, architecture shifts
- **Minor (Y):** New features, backward compatible
- **Patch (Z):** Bug fixes, small improvements

Pre-release: `0.Y.Z` for development, `X.Y.Z-rc.N` for release candidates

---

## Tagging Convention

- Git tags: `vX.Y.Z` (e.g., `v0.1.0`)
- Branches: `feature/*`, `fix/*`, `release/*`
- Main branch: `main` (stable), `dev` (development)

---

*This changelog tracks user-visible changes. For technical implementation details, see commit history and TASKS.md.*