# CoreDeck Dependencies and Licenses

**Version:** 0.2  
**Status:** DRAFT - verify every licence against the official source before shipping  
**Last Updated:** 2026-10-10

---

## CoreDeck License

**GPL-3.0-or-later** — Inherited from [Throne](https://github.com/throneproj/Throne/blob/dev/LICENSE)

> CoreDeck is derived from Throne and absorbs the required Throne source into its own tree (DEC-011). All modifications and additions are GPL-3.0-or-later. Upstream copyright and licence headers are preserved. The full licence text ships as `LICENSE` in the CoreDeck repository root (copied from the upstream Throne repository at integration time). Attribution to Throne and its upstream authors is kept in `README.md` and in this file.

---

## Direct Dependencies (Bundled/Linked)

### Qt 6 (LGPL-3.0 / Commercial)
| Component | License | Use |
|-----------|---------|-----|
| Qt Core | LGPL-3.0 | Application foundation |
| Qt Widgets | LGPL-3.0 | GUI |
| Qt Network | LGPL-3.0 | Networking, RPC sockets |
| Qt LinguistTools | LGPL-3.0 | Translations |
| Qt DBus | LGPL-3.0 (verify module licence in Qt docs) | Linux system integration |
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

### Xray-core (MPL-2.0)
| Source | License | Use |
|--------|---------|-----|
| https://github.com/XTLS/Xray-core | MPL-2.0 (earlier draft said GPL-3.0/MPL-2.0 — corrected; re-verify on the official repo) | Default core |
| **Distribution** | | Downloaded at runtime, not bundled |
| **Compliance** | | MPL-2.0 is GPL-3.0 compatible; keep source link and licence notice |

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
| Wintun | Wintun prebuilt-binaries licence (NOT GPL; redistribution terms differ) | TUN driver (P2). Read the licence before bundling |
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
- [ ] Throne attribution and upstream LICENSE present in the repository root

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
| Qt | 6.5+ | 6.6+ | Pin in CI (Windows + Linux) |
| sing-box | Latest stable | Latest stable | Auto-update |
| Xray-core | Latest stable | Latest stable | Auto-update |
| SQLiteCpp | Bundled | Bundled | Copied into CoreDeck `3rdparty/` |
| simple-protobuf | Bundled | Bundled | Copied into CoreDeck `3rdparty/` |
| fkYAML | Bundled | Bundled | Copied into CoreDeck `3rdparty/` |

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
| 2026-10-10 | Review | AI | Xray-core corrected to MPL-2.0, Wintun licence corrected, macOS removed, Throne absorbed. Still to do at integration (MI-119): re-read each bundled `3rdparty/*/LICENSE`, list actually copied components, add attribution |

---

*Keep this file updated with every dependency change. Run license check before each release.*