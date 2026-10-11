# CoreDeck Build and Test Guide

**Version:** 0.2  
**Status:** BLOCKED - toolchain not yet provisioned (M0 not reached)  
**Last Updated:** 2026-10-10  
**Platforms:** Windows 10/11 (primary), Linux. macOS is out of scope (DEC-012).

> Builds must work from a clean checkout of the CoreDeck repository alone. No `Throne/` folder or external path is required (DEC-011).

---

## 1. Prerequisites

### Windows
| Tool | Version | Install (non-interactive) |
|------|---------|---------------------------|
| Git | 2.40+ | already present (2.49.0) |
| CMake | 3.20+ | `winget install Kitware.CMake` |
| Ninja | 1.11+ | `winget install Ninja-build.Ninja` |
| MSVC Build Tools 2022 | 17.6+ | `winget install Microsoft.VisualStudio.2022.BuildTools --override "--passive --wait --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"` |
| Qt | 6.5+ (6.6+ preferred) | `pip install aqtinstall` then `aqt install-qt windows desktop 6.6.3 win64_msvc2019_64 -O C:\Qt` |
| Python | 3.10+ | `winget install Python.Python.3.12` |
| 7-Zip | any | `winget install 7zip.7zip` (packaging) |

MinGW-w64 is an alternative compiler (use the matching Qt `win64_mingw` arch).

### Linux
| Tool | Version | Notes |
|------|---------|-------|
| Git, CMake 3.20+, Ninja 1.11+ | | package manager |
| GCC 11+ / Clang 14+ | | C++20 |
| Qt 6.5+ | | e.g. Debian/Ubuntu: `qt6-base-dev qt6-tools-dev qt6-l10n-tools libqt6dbus6` |
| SQLite dev headers | 3.38+ | SQLiteCpp is bundled |

Example: `sudo apt install -y build-essential cmake ninja-build qt6-base-dev qt6-tools-dev qt6-l10n-tools libgl1-mesa-dev`

---

## 2. Environment Verification Log

### 2026-10-10 (Windows 11 Pro 10.0.26300, x64)
| Item | Result |
|------|--------|
| Git | OK 2.49.0.windows.1 |
| CMake | MISSING |
| Qt6 | MISSING (no `C:\Qt`, `C:\Qt6`, `C:\Tools\Qt`) |
| MSVC / VS Build Tools 2022 | MISSING |
| MinGW-w64 | MISSING |
| Ninja | MISSING |
| winget / Chocolatey / Scoop | winget availability NOT YET CHECKED (earlier note only said Chocolatey/Scoop absent) |

**Status:** M0 not reached. Next action (done by the AI, no owner input): run `winget --version`; install the tools above; if that fails, use the GitHub Actions workflow (section 6) and cite its run as evidence.

Add a new dated entry here every time the environment is re-checked.

---

## 3. Build (CoreDeck root)

### Windows (MSVC, Ninja)
Run from a "x64 Native Tools" prompt (or after `vcvars64.bat`):
```powershell
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=C:\Qt\6.6.3\msvc2019_64
cmake --build build
```

### Windows (MinGW)
```powershell
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_PREFIX_PATH=C:\Qt\6.6.3\mingw_64
cmake --build build
```

### Linux
```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Expected artifacts
| Platform | Output |
|----------|--------|
| Windows | `build/CoreDeck.exe` (+ Qt DLLs via `windeployqt`) |
| Linux | `build/CoreDeck` |

(Binary name is decided at integration time; update this table when it is.)

### Build verification results
| Date | Step | Command | Result | Notes |
|------|------|---------|--------|-------|
| 2026-10-10 | env | `cmake --version` | FAILED | not installed |
| 2026-10-10 | env | `where qmake` | FAILED | not installed |
| 2026-10-10 | env | `where cl` | FAILED | not installed |
| 2026-10-10 | configure | `cmake -S . -B build -G Ninja` | NOT RUN | blocked by toolchain |
| 2026-10-10 | build | `cmake --build build` | NOT RUN | blocked by toolchain |

Also note: no root `CMakeLists.txt` exists yet; creating it is part of the Throne integration task (MI-119).

---

## 4. Running

```powershell
# Windows
.\build\CoreDeck.exe
.\build\CoreDeck.exe -appdata .\config     # portable data dir
```
```bash
# Linux
./build/CoreDeck
./build/CoreDeck -appdata ~/.config/coredeck
```

| Argument | Description |
|----------|-------------|
| `-appdata <path>` | Use a custom config directory |
| `-many` | Allow multiple GUI processes (development only) |
| `-tray` | Start minimized to tray |
| `-debug` | Debug logging |
| `-flag_restart_tun_on` | Restore TUN on start |

---

## 5. Testing

### Unit and integration tests (CTest)
```bash
ctest --test-dir build --output-on-failure
```
Tests use temporary databases and fixtures only. Record the pass/fail counts below after every real run.

| Date | Command | Passed | Failed | Notes |
|------|---------|--------|--------|-------|
| — | — | — | — | No test has been executed yet |

Existing test sources (written, NOT compiled or run): `tests/test_instance.cpp`, `tests/test_instancemanager.cpp`. They count as `DONE_UNVERIFIED` until a CTest run is recorded. The migration, `InstanceRepository`, and `PortAllocator` tests with Throne-format fixtures are still to be written.

### Manual smoke tests

**S1 Startup:** launch; UI loads; log shows data dir; Default Instance exists; close cleanly; no orphan core processes.  
**S2 Profile/subscription:** add subscription (use `YOUR_SUBSCRIPTION_URL` placeholders in docs), update, profiles appear, start/stop a profile, logs show connection.  
**S3 System proxy:** enable, browser uses proxy, disable, browser direct.  
**S4 TUN (Windows/Linux, needs admin/root, P2):** enable, verify routing, disable, verify cleanup.

### Multi-instance tests
- **MI-1 Two Instances:** create A and B, start both, different PIDs/ports, different exit IPs through each port.
- **MI-2 Isolation:** kill A's core PID; B keeps running; A restarts only if `restart_on_crash`.
- **MI-3 Port conflict:** two Instances with the same SOCKS port; start A then B; B shows a clear port-conflict error and the GUI stays up.
- **MI-4 Subscription independence:** same URL in A and B; update/delete in A; B unchanged.
- **MI-5 Auto-start recovery:** A `auto_start=true`, B false; restart app; A running with last profile restored, B stopped.
- **MI-6 Migration:** open a Throne-format DB fixture copy; verify Default Instance, all Groups/Profiles/Settings preserved, `.pre-coredeck.bak` created, re-run is idempotent.

---

## 6. CI (GitHub Actions)

CoreDeck owns `.github/workflows/build.yml` (to be created if local tools cannot be installed; recommended anyway).

Matrix: `windows-latest` (MSVC, Qt via `jurplel/install-qt-action`) and `ubuntu-latest` (apt Qt6). Steps: checkout, configure, build, `ctest --output-on-failure`, upload artifact (Windows portable ZIP, Linux binary).

A green run on the pushed commit, with its log cited in the session report, counts as build/test evidence. Do not copy Throne's macOS jobs.

---

## 7. Performance Targets

| Metric | Target | Method |
|--------|--------|--------|
| Memory, 1 Instance idle | < 60 MB | Task Manager / `ps` |
| Memory, 5 Instances idle | < 200 MB | same |
| CPU, 5 Instances idle | < 2% | Task Manager / `top` |
| Startup, 5 auto-start | < 10 s | stopwatch |
| Port bind success | 100% | automated test |
| Core crash isolation | 100% | kill test |

---

## 8. Packaging

| Platform | Deliverable |
|----------|-------------|
| Windows | Portable ZIP `CoreDeck-{version}-win64.zip` (exe + Qt DLLs via `windeployqt` + licences; cores downloaded at runtime). Installer (NSIS/WiX) later. |
| Linux | AppImage / `.deb` / `.rpm` (P1; a built binary from CI is enough for the prototype) |

Every package includes: GPL-3.0 text, third-party licence notices, source-offer link (see DEPENDENCIES_AND_LICENSES.md).

---

## 9. Troubleshooting

| Issue | Solution |
|-------|----------|
| CMake can't find Qt | Set `CMAKE_PREFIX_PATH` to the Qt kit directory |
| MSVC `/Zi` vs `/Z7` | Handled in CMakeLists for RelWithDebInfo |
| SQLiteCpp linker errors | Ensure `BUILD_SHARED_LIBS=OFF` |
| Core not found | Check the core folder or let the app download it on first run |
| RPC connection failed | Check firewall, local socket name, permissions |
| TUN needs privileges | Run elevated (Windows) / root or polkit (Linux) |
| `winget` not found | Install "App Installer" from Microsoft Store, or use the CI workflow |

---

## 10. Build Verification Checklist

- [ ] CMake configure succeeds
- [ ] Build completes without errors
- [ ] CTest passes (counts recorded)
- [ ] Executable runs and shows UI
- [ ] Default Instance created / Throne DB migrated on fixture
- [ ] Can add subscription, start/stop a profile
- [ ] Logs show RPC connection
- [ ] System Proxy toggle works
- [ ] Settings persist after restart

---

*Update this file after every build/test session with the real commands and results.*
