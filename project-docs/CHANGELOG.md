# CoreDeck Changelog

**Version:** 0.1 (Development)  
**Format:** Based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)

---

## [Unreleased] - Development

### Added
- **MI-020: InstanceManager Singleton (Session 005)**
  - InstanceManager singleton for Instance lifecycle management
  - Instance CRUD: create, get (by ID/name), getAll, update, delete
  - Active Instance management with QSettings persistence
  - Default Instance creation on first run
  - Auto-start Instance query support
  - Thread-safe operations via std::mutex
  - Instance cache with invalidation on modifications
  - Duplicate name prevention and last-instance deletion protection
  - Unit tests for interface contracts and logic flow
- **MI-011: Database Schema & Migration (Phase 1 Complete)**
  - `instances` table with UUID primary key, all Instance fields, timestamps
  - `instance_settings` table for per-instance settings (FK to instances)
  - `port_allocations` table for central port tracking with unique port index
  - `groups.instance_id` column with foreign key to instances
  - Throne→CoreDeck in-place migration logic (run on first launch)
  - Default Instance creation with all existing Throne data migrated
  - Migration version tracking in settings
  - InstanceRepository with full CRUD operations
  - Thread-safe database operations via WriteMutex
- **MI-010: Instance Data Model (Session 002)**
  - Instance entity class with all fields from PROJECT_SPEC.md Section 3
  - CoreType enum: Xray (default), SingBox, External
  - Nested InstanceCoreConfig and InstanceDNSSettings structs
  - JSON serialization/deserialization (ToJson/FromJson)
  - Validation methods (IsValid, Validate)
  - UUID primary key generation and preservation
  - Equality operators based on UUID identity
  - 13 comprehensive unit tests covering all acceptance criteria
- Project documentation structure (`project-docs/`)
- `README.md` - Project overview, quick start, documentation index
- `PROJECT_SPEC.md` - Complete product specification with 17 sections
- `ARCHITECTURE.md` - Current Throne architecture analysis + target CoreDeck architecture
- `ROADMAP.md` - 11-phase development plan with detailed tasks
- `TASKS.md` - 117 tasks with acceptance criteria and test methods
- `PROGRESS.md` - Current status tracker
- CoreDeck Git repository initialized at `G:\Project\CoreDeck\`
- Throne upstream cloned for reference at `G:\Project\CoreDeck\Throne\`

### Changed
- **Repository URL corrected** (Session 003): Git remote updated from `DevastBit/CoreDeck` to `gameacc2025/CoreDeck`
- Documentation links updated in README.md and PROGRESS.md

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