# CoreDeck Build and Test Guide

**Version:** 0.1  
**Status:** BLOCKED - Missing Toolchain  
**Last Updated:** 2026-10-10

---

## Prerequisites

### Windows
| Tool | Version | Notes | Available |
|------|---------|-------|-----------|
| Git | 2.40+ | | ✅ 2.49.0 |
| CMake | 3.20+ | | ❌ Not installed |
| Qt | 6.5+ (6.6+ recommended) | MSVC 2022 64-bit or MinGW-w64 | ❌ Not installed |
| Visual Studio | 2022 17.6+ | Community/Pro/Enterprise | ❌ Not installed |
| Ninja | 1.11+ | Optional, faster builds | ❌ Not installed |
| Python | 3.10+ | For install scripts | ❓ Unknown |
| 7-Zip | | For packaging | ❓ Unknown |

### Linux
| Tool | Version | Notes |
|------|---------|-------|
| Git | 2.40+ | |
| CMake | 3.20+ | |
| Qt | 6.5+ | `qt6-base-dev`, `qt6-tools-dev` |
| GCC | 11+ / Clang 14+ | C++20 support |
| Ninja | 1.11+ | Recommended |
| sqlite3 | 3.38+ | Dev headers |
| protobuf | 3.21+ | For simple-protobuf (bundled) |

### macOS
| Tool | Version | Notes |
|------|---------|-------|
| Git | 2.40+ | |
| CMake | 3.20+ | |
| Qt | 6.5+ | `brew install qt6` |
| Xcode | 14.3+ | Command Line Tools |
| Ninja | 1.11+ | `brew install ninja` |

---

## Environment Verification (2026-10-10)

**OS:** Windows 11 Pro (10.0.26300 Build 26300)  
**Architecture:** x64  

**Toolchain Status:**
- Git: ✅ Available (2.49.0.windows.1)
- CMake: ❌ Not in PATH, not installed
- Qt6: ❌ Not installed (no `C:\Qt\`, `C:\Qt6\`, `C:\Tools\Qt\`)
- Visual Studio 2022: ❌ Not installed (no `C:\Program Files\Microsoft Visual Studio\2022\`)
- Visual Studio Build Tools: ❌ Not installed
- MinGW-w64: ❌ Not in PATH
- Ninja: ❌ Not in PATH
- Chocolatey/Scoop: ❌ Not available

**Conclusion:** The current Windows environment lacks the complete C++/Qt toolchain required to build Throne. This is a **blocker** for MI-002 (Verify baseline build).

**Required to proceed:**
1. Install Visual Studio 2022 Community/Build Tools with "Desktop development with C++" workload
2. Install Qt 6.6+ (MSVC 2022 64-bit kit) via Qt Online Installer
3. Install CMake 3.20+ and add to PATH
4. Install Ninja 1.11+ and add to PATH

Alternatively, use a Windows machine/VM with the toolchain pre-installed, or use GitHub Actions for CI builds.

---

## Throne Baseline Build (Verification)

### Windows (MSVC)
```powershell
cd Throne
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
ninja
# Or with VS:
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

### Windows (MinGW)
```powershell
cd Throne
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
ninja
```

### Linux
```bash
cd Throne
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
ninja
```

### macOS
```bash
cd Throne
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$(brew --prefix qt6)" ..
ninja
```

### Expected Artifacts
| Platform | Output |
|----------|--------|
| Windows | `build/Throne.exe` (+ DLLs in same dir) |
| Linux | `build/Throne` (AppImage via script) |
| macOS | `build/Throne.app` |

---

## Build Verification Results (2026-10-10)

| Step | Command | Result | Notes |
|------|---------|--------|-------|
| Environment check | `cmake --version` | FAILED | CMake not installed |
| Environment check | `where qmake` | FAILED | Qt not installed |
| Environment check | `where cl` | FAILED | MSVC not installed |
| CMake configure | `cmake -G "Ninja" ..` | NOT RUN | Blocked by missing tools |
| Build | `ninja` | NOT RUN | Blocked by missing tools |

**Status:** BLOCKED - Cannot proceed with build verification until toolchain is installed.

---

## CoreDeck Build (Future)

*To be documented after Phase 1 implementation.*

### Linux
| Tool | Version | Notes |
|------|---------|-------|
| Git | 2.40+ | |
| CMake | 3.20+ | |
| Qt | 6.5+ | `qt6-base-dev`, `qt6-tools-dev` |
| GCC | 11+ / Clang 14+ | C++20 support |
| Ninja | 1.11+ | Recommended |
| sqlite3 | 3.38+ | Dev headers |
| protobuf | 3.21+ | For simple-protobuf (bundled) |

### macOS
| Tool | Version | Notes |
|------|---------|-------|
| Git | 2.40+ | |
| CMake | 3.20+ | |
| Qt | 6.5+ | `brew install qt6` |
| Xcode | 14.3+ | Command Line Tools |
| Ninja | 1.11+ | `brew install ninja` |

---

## Throne Baseline Build (Verification)

### Windows (MSVC)
```powershell
cd Throne
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
ninja
# Or with VS:
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

### Windows (MinGW)
```powershell
cd Throne
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
ninja
```

### Linux
```bash
cd Throne
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
ninja
```

### macOS
```bash
cd Throne
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$(brew --prefix qt6)" ..
ninja
```

### Expected Artifacts
| Platform | Output |
|----------|--------|
| Windows | `build/Throne.exe` (+ DLLs in same dir) |
| Linux | `build/Throne` (AppImage via script) |
| macOS | `build/Throne.app` |

---

## CoreDeck Build (Future)

*To be documented after Phase 1 implementation.*

```bash
# From CoreDeck root (not Throne/)
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
ninja
```

---

## Running the Application

### Windows
```powershell
# From build directory
.\Throne.exe
# Or portable:
.\Throne.exe -appdata .\config
```

### Linux
```bash
./Throne
# Or with custom config dir:
./Throne -appdata ~/.config/coredeck
```

### macOS
```bash
open ./Throne.app
# Or CLI:
./Throne.app/Contents/MacOS/Throne
```

### Common Arguments
| Argument | Description |
|----------|-------------|
| `-appdata <path>` | Use custom config directory |
| `-many` | Allow multiple GUI instances (dev) |
| `-tray` | Start minimized to tray |
| `-debug` | Enable debug logging |
| `-flag_restart_tun_on` | Restore TUN on start |

---

## Testing

### Unit Tests (Planned)
```bash
# After test infrastructure added
cd build
ctest --output-on-failure
```

### Integration Tests (Manual)

#### Test 1: Basic Startup
1. Launch Throne
2. Verify UI loads without errors
3. Check log for "appdata mode" message
4. Close normally

#### Test 2: Profile Management
1. Add subscription URL
2. Update subscription
3. Verify profiles appear
4. Test profile latency
5. Start profile
6. Verify connection (check logs)
7. Stop profile

#### Test 3: System Proxy
1. Enable System Proxy
2. Verify browser traffic routes through proxy
3. Disable System Proxy
4. Verify browser direct connection

#### Test 4: TUN Mode (Linux/Windows)
1. Enable TUN (requires admin/root)
2. Verify system-wide routing
3. Disable TUN
5. Verify cleanup

### Multi-Instance Tests (CoreDeck - Future)

#### Test MI-1: Two Instances Running
1. Create Instance "A" and "B"
2. Configure different subscriptions
3. Start both Instances
4. Verify both show "Running" with different PIDs
5. Connect Browser A to Instance A port
6. Connect Browser B to Instance B port
7. Verify different exit IPs

#### Test MI-2: Instance Isolation
1. Start Instance A and B
2. Crash Instance A Core (kill PID)
3. Verify Instance B still running
4. Verify Instance A auto-restarts (if enabled)

#### Test MI-3: Port Conflict Handling
1. Create Instance A with SOCKS 10808
2. Create Instance B with SOCKS 10808
3. Start A, then try to start B
4. Verify B shows port conflict error

#### Test MI-4: Subscription Independence
1. Instance A: Subscribe to URL X
2. Instance B: Subscribe to same URL X
3. Update in A
4. Verify B's profiles unchanged
5. Delete subscription in A
6. Verify B's subscription intact

#### Test MI-5: Auto-Start Recovery
1. Configure Instance A: auto_start=true
2. Configure Instance B: auto_start=false
3. Close app
4. Reopen app
5. Verify A running, B stopped
6. Verify A restored last profile

---

## Performance Benchmarks (Target)

| Metric | Target | Measurement Method |
|--------|--------|-------------------|
| Memory (1 Instance idle) | < 60 MB | Task Manager / `ps` |
| Memory (5 Instances idle) | < 200 MB | Task Manager / `ps` |
| CPU (5 Instances idle) | < 2% | Task Manager / `top` |
| Startup (5 auto-start) | < 10 seconds | Stopwatch |
| Port bind success | 100% | Automated test |
| Core crash isolation | 100% | Kill test |

---

## Packaging

### Windows
- Portable ZIP: `Throne-{version}-win64.zip` (exe + DLLs + cores)
- Installer: NSIS or WiX (future)

### Linux
- AppImage: `script/build_appimage.sh`
- DEB: `cmake --build . --target package_deb`
- RPM: `cmake --build . --target package_rpm`

### macOS
- DMG: `script/build_dmg.sh`
- Notarization: `xcrun notarytool submit`

---

## CI/CD (Reference)

Throne uses GitHub Actions. See `.github/workflows/` for:
- Windows build (MSVC + MinGW)
- Linux build (Ubuntu, Arch)
- macOS build
- Release packaging

---

## Troubleshooting

| Issue | Solution |
|-------|----------|
| CMake can't find Qt | Set `CMAKE_PREFIX_PATH` to Qt installation |
| MSVC: `/Zi` vs `/Z7` | CMakeLists handles this for RelWithDebInfo |
| Linker errors (SQLiteCpp) | Ensure `BUILD_SHARED_LIBS=OFF` |
| Core not found | Check `3rdparty/` or auto-download on first run |
| RPC connection failed | Check firewall, socket permissions |
| TUN requires root | Run with sudo or configure polkit |

---

## Build Verification Checklist

- [ ] CMake configure succeeds
- [ ] Build completes without errors
- [ ] Executable runs and shows UI
- [ ] Can add subscription
- [ ] Can start/stop profile
- [ ] Logs show RPC connection
- [ ] System Proxy toggle works
- [ ] Settings persist after restart

---

*Update this file after every build/test session. Record actual commands and results.*