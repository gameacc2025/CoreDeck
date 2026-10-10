# CoreDeck Dependencies and Licenses

**Version:** 0.1  
**Status:** DRAFT - Based on Throne Analysis  
**Last Updated:** 2026-10-10

---

## CoreDeck License

**GPL-3.0-or-later** — Inherited from [Throne](https://github.com/throneproj/Throne/blob/dev/LICENSE)

> CoreDeck is a fork of Throne. All modifications and additions are licensed under GPL-3.0-or-later.
> See [Throne's LICENSE](Throne/LICENSE) for full text.

---

## Direct Dependencies (Bundled/Linked)

### Qt 6 (LGPL-3.0 / Commercial)
| Component | License | Use |
|-----------|---------|-----|
| Qt Core | LGPL-3.0 | Application foundation |
| Qt Widgets | LGPL-3.0 | GUI |
| Qt Network | LGPL-3.0 | Networking, RPC sockets |
| Qt LinguistTools | LGPL-3.0 | Translations |
| Qt DBus | LGPL-3.0 | Linux system integration |
| **Compliance** | | Dynamic linking, provide Qt libraries, allow user swap |

### SQLiteCpp (MIT)
| File | License | Use |
|------|---------|-----|
| `3rdparty/SQLiteCpp/` | MIT | SQLite wrapper |
| `3rdparty/SQLiteCpp/src/sqlite3.c` | Public Domain | SQLite engine |
| **Compliance** | | Include license in distribution |

### simple-protobuf (MIT)
| File | License | Use |
|------|---------|-----|
| `3rdparty/simple-protobuf/` | MIT | Protobuf serialization for RPC |
| **Compliance** | | Include license in distribution |

### fkYAML (MIT)
| File | License | Use |
|------|---------|-----|
| `3rdparty/fkYAML/` | MIT | YAML parsing for configs |
| **Compliance** | | Include license in distribution |

### quirc (ISC)
| File | License | Use |
|------|---------|-----|
| `3rdparty/quirc/` | ISC | QR code decoding |
| **Compliance** | | Include license in distribution |

### monocypher (CC0 / Public Domain)
| File | License | Use |
|------|---------|-----|
| `3rdparty/monocypher/` | CC0 | Cryptographic primitives |
| **Compliance** | | No attribution required |

### qv2ray components (GPL-3.0)
| File | License | Use |
|------|---------|-----|
| `3rdparty/qv2ray/v2/ui/` | GPL-3.0 | UI widgets (LogHighlighter, etc.) |
| **Compliance** | | Compatible with GPL-3.0 |

### qrcodegen (MIT)
| File | License | Use |
|------|---------|-----|
| `3rdparty/qrcodegen.cpp` | MIT | QR code generation |
| **Compliance** | | Include license in distribution |

### QtExtKeySequenceEdit (MIT)
| File | License | Use |
|------|---------|-----|
| `3rdparty/QtExtKeySequenceEdit.cpp` | MIT | Key sequence editor widget |
| **Compliance** | | Include license in distribution |

---

## Runtime Dependencies (Core Binaries)

### sing-box (GPL-3.0)
| Source | License | Use |
|--------|---------|-----|
| https://github.com/SagerNet/sing-box | GPL-3.0 | Primary proxy core |
| **Distribution** | | Downloaded at runtime, not bundled |
| **Compliance** | | Source available, license conveyed |

### Xray-core (GPL-3.0 / MPL-2.0)
| Source | License | Use |
|--------|---------|-----|
| https://github.com/XTLS/Xray-core | GPL-3.0 / MPL-2.0 | Xray-specific protocols |
| **Distribution** | | Downloaded at runtime, not bundled |
| **Compliance** | | Dual license: GPL-3.0 compatible |

### GeoIP / GeoSite Data
| Source | License | Use |
|--------|---------|-----|
| Xray/Loyalsoldier geoip.dat/geosite.dat | Various (CC-BY, etc.) | Routing rules |
| **Distribution** | | Downloaded at runtime |
| **Compliance** | | Respect individual licenses |

---

## System Dependencies

### Windows
| Dependency | License | Notes |
|------------|---------|-------|
| Wintun | GPL-3.0 | TUN driver (future) |
| WinPcap/Npcap | BSD / Proprietary | Packet capture (optional) |
| Visual C++ Redistributable | Microsoft | Runtime |

### Linux
| Dependency | License | Notes |
|------------|---------|-------|
| glibc | LGPL-2.1+ | System C library |
| libcap | BSD-3-Clause | Capabilities for TUN |
| polkit | LGPL-2.1+ | Privilege escalation |
| systemd | LGPL-2.1+ | Service integration |
| dbus | GPL-2.0+ / AFL | IPC |

### macOS
| Dependency | License | Notes |
|------------|---------|-------|
| NetworkExtension | Apple | TUN (future, requires entitlements) |
| AppKit / Foundation | Apple | System frameworks |

---

## Build Dependencies

| Tool | License | Purpose |
|------|---------|---------|
| CMake | BSD-3-Clause | Build system |
| Ninja | Apache-2.0 | Build tool |
| Python 3 | PSF | Install scripts |
| Git | GPL-2.0 | Version control |

---

## License Compliance Checklist

### For Distribution (Binary Packages)
- [ ] Include GPL-3.0 license text
- [ ] Include all third-party licenses (MIT, BSD, ISC, etc.)
- [ ] Provide source code offer (written offer or download link)
- [ ] Ensure Qt dynamically linked (or provide object files for static)
- [ ] Document Core binary sources (sing-box, Xray-core URLs)
- [ ] Verify no GPL-incompatible code linked

### For Source Distribution
- [ ] All bundled third-party code includes licenses
- [ ] CMakeLists.txt doesn't fetch non-free dependencies
- [ ] Submodules/patches have compatible licenses

### Runtime
- [ ] Core binaries downloaded from official sources
- [ ] GeoIP/GeoSite from official sources
- [ ] No telemetry or unauthorized data collection
- [ ] User controls all network connections

---

## Dependency Version Tracking

| Dependency | Version (Throne) | CoreDeck Target | Notes |
|------------|------------------|-----------------|-------|
| Qt | 6.5+ | 6.6+ | Pin in CI |
| sing-box | Latest stable | Latest stable | Auto-update |
| Xray-core | Latest stable | Latest stable | Auto-update |
| SQLiteCpp | Bundled | Bundled | Update with Throne |
| simple-protobuf | Bundled | Bundled | Update with Throne |
| fkYAML | Bundled | Bundled | Update with Throne |

---

## Adding New Dependencies

**Process:**
1. Verify license compatibility (GPL-3.0 compatible)
2. Add to `DEPENDENCIES_AND_LICENSES.md`
3. Update CMakeLists.txt with proper `find_package` or bundled source
4. Document in `BUILD_AND_TEST.md`
5. Test on all target platforms

**Prohibited:**
- Proprietary/closed-source dependencies
- GPL-incompatible licenses (e.g., OpenSSL without exception)
- Runtime dependencies without source availability

---

## License Resources

| Resource | URL |
|----------|-----|
| GPL-3.0 | https://www.gnu.org/licenses/gpl-3.0.html |
| LGPL-3.0 | https://www.gnu.org/licenses/lgpl-3.0.html |
| MIT | https://opensource.org/licenses/MIT |
| BSD-3-Clause | https://opensource.org/licenses/BSD-3-Clause |
| Apache-2.0 | https://www.apache.org/licenses/LICENSE-2.0 |
| MPL-2.0 | https://www.mozilla.org/en-US/MPL/2.0/ |

---

## Audit Log

| Date | Action | By | Notes |
|------|--------|----|-------|
| 2026-10-10 | Initial audit from Throne source | AI | Based on Throne CMakeLists.txt and 3rdparty/ |

---

*Keep this file updated with every dependency change. Run license check before each release.*