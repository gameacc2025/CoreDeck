# CoreDeck source files

# Include directory for generated protobuf headers
include_directories(${CMAKE_BINARY_DIR})

# Qt resource files
qt6_add_resources(COREDECK_QRC
    ${CMAKE_SOURCE_DIR}/res/Throne.qrc
    ${CMAKE_SOURCE_DIR}/res/darkstyle.qrc
    ${CMAKE_SOURCE_DIR}/res/EmojiFont.qrc
    ${CMAKE_SOURCE_DIR}/res/EmojiFontWin.qrc
)

# Database entities
set(ENTITY_SOURCES
    src/database/entities/Instance.cpp
    src/database/entities/Group.cpp
    src/database/entities/Profile.cpp
    src/database/entities/RouteProfile.cpp
    src/database/entities/RouteRule.cpp
    src/database/entities/OtpProfile.cpp
    src/database/entities/IpScan.cpp
    src/database/entities/EndpointSource.cpp
)

# Database repositories
set(REPO_SOURCES
    src/database/Database.cpp
    src/database/DatabaseManager.cpp
    src/database/Migration.cpp
    src/database/InstanceRepository.cpp
    src/database/GroupsRepo.cpp
    src/database/ProfilesRepo.cpp
    src/database/RoutesRepo.cpp
    src/database/SettingsRepo.cpp
    src/database/TrafficStatsRepo.cpp
    src/database/MarkersRepo.cpp
    src/database/OtpProfilesRepo.cpp
    src/database/IpListsRepo.cpp
    src/database/IpScansRepo.cpp
)

# Global utilities
set(GLOBAL_SOURCES
    src/global/Configs.cpp
    src/global/Utils.cpp
    src/global/Logger.cpp
    src/global/PeriodicRunner.cpp
    src/global/HTTPRequestHelper.cpp
    src/global/DeviceDetailsHelper.cpp
    src/global/CountryHelper.cpp
    src/global/LocalNetwork.cpp
    src/global/OTP.cpp
    src/global/OtpPlaceholder.cpp
    src/global/VpnCredentialOverride.cpp
    src/global/RunningProfiles.cpp
    src/global/Common.cpp
)

# Instance management
set(INSTANCE_SOURCES
    src/instance/InstanceManager.cpp
)

# Configs
set(CONFIG_SOURCES
    src/configs/AutoSelectorPlan.cpp
    src/configs/common/DialFields.cpp
    src/configs/common/QUICFields.cpp
    src/configs/common/TLS.cpp
    src/configs/common/multiplex.cpp
    src/configs/common/transport.cpp
    src/configs/common/Outbound.cpp
    src/configs/common/OutboundFactory.cpp
    src/configs/common/utils.cpp
    src/configs/common/xrayMultiplex.cpp
    src/configs/common/xrayStreamSetting.cpp
    src/configs/outbounds/anyTLS.cpp
    src/configs/outbounds/autoselector.cpp
    src/configs/outbounds/http.cpp
    src/configs/outbounds/hysteria.cpp
    src/configs/outbounds/shadowsocks.cpp
    src/configs/outbounds/socks.cpp
    src/configs/outbounds/ssh.cpp
    src/configs/outbounds/tailscale.cpp
    src/configs/outbounds/trojan.cpp
    src/configs/outbounds/tuic.cpp
    src/configs/outbounds/juicity.cpp
    src/configs/outbounds/trusttunnel.cpp
    src/configs/outbounds/naive.cpp
    src/configs/outbounds/shadowtls.cpp
    src/configs/outbounds/vless.cpp
    src/configs/outbounds/vmess.cpp
    src/configs/outbounds/wireguard.cpp
    src/configs/outbounds/masque.cpp
    src/configs/outbounds/mieru.cpp
    src/configs/outbounds/openvpn.cpp
    src/configs/outbounds/openconnect.cpp
    src/configs/outbounds/custom.cpp
    src/configs/generate.cpp
    src/configs/sub/GroupUpdater.cpp
    src/configs/sub/SubscriptionParser.cpp
    src/configs/sub/SubscriptionReconcile.cpp
    src/configs/sub/RouteUpdater.cpp
    src/configs/sub/clash.cpp
    src/configs/sub/vpnFileImport.cpp
    src/configs/sub/warp.cpp
)

# API
set(API_SOURCES
    src/api/RPC.cpp
    src/api/remote/Protocol.cpp
    src/api/remote/Message.cpp
    src/api/remote/Server.cpp
    src/api/remote/Config.cpp
    src/api/remote/Router.cpp
    src/api/remote/RouterTests.cpp
)

# Scanner
set(SCANNER_SOURCES
    src/scanner/DefaultIpLists.cpp
    src/scanner/IpListParse.cpp
    src/scanner/IpListUpdater.cpp
    src/scanner/ScanManager.cpp
    src/scanner/ScanProfiles.cpp
    src/scanner/ScanWorker.cpp
)

# Stats
set(STATS_SOURCES
    src/stats/traffic/TrafficLooper.cpp
    src/stats/traffic/TrafficStatsManager.cpp
    src/stats/autoselector/AutoSelectorMonitor.cpp
    src/stats/connectionLister/connectionLister.cpp
)

# Sys
if (WIN32)
    set(SYS_SOURCES
        src/sys/Process.cpp
        src/sys/KillSwitch.cpp
        src/sys/ProcessMetrics.cpp
        src/sys/CoreDiagnostics.cpp
        src/sys/UrlScheme.cpp
        src/sys/SystemProxy.cpp
        src/sys/GlobalHotkeys.cpp
        src/sys/coreDump.cpp
        src/sys/LinuxCap.cpp
        src/sys/windows/AutoRun.cpp
        src/sys/windows/eventHandler.cpp
        src/sys/windows/GlobalHotkeys.cpp
        src/sys/windows/guihelper.cpp
        src/sys/windows/MiniDump.cpp
        src/sys/windows/SystemProxy.cpp
        src/sys/windows/UrlScheme.cpp
        src/sys/windows/WinVersion.cpp
    )
else ()
    set(SYS_SOURCES
        src/sys/Process.cpp
        src/sys/KillSwitch.cpp
        src/sys/ProcessMetrics.cpp
        src/sys/CoreDiagnostics.cpp
        src/sys/UrlScheme.cpp
        src/sys/SystemProxy.cpp
        src/sys/GlobalHotkeys.cpp
        src/sys/coreDump.cpp
        src/sys/LinuxCap.cpp
        src/sys/linux/AutoRun.cpp
        src/sys/linux/GlobalHotkeysKde.cpp
        src/sys/linux/GlobalHotkeysX11.cpp
        src/sys/linux/SystemProxy.cpp
        src/sys/linux/UrlScheme.cpp
        src/sys/linux/systemChecks.cpp
    )
endif ()

# UI
set(UI_SOURCES
    src/ui/mainWindow/mainwindow_setup.cpp
    src/ui/mainWindow/mainwindow_events.cpp
    src/ui/mainWindow/mainwindow_groups.cpp
    src/ui/mainWindow/mainwindow_view.cpp
    src/ui/mainWindow/mainwindow_connections.cpp
    src/ui/mainWindow/mainwindow_profiles.cpp
    src/ui/mainWindow/mainwindow_deeplink.cpp
    src/ui/mainWindow/mainwindow_system.cpp
    src/ui/mainWindow/mainwindow_log.cpp
    src/ui/mainWindow/mainwindow_hotkeys.cpp
    src/ui/mainWindow/mainwindow_profile_lifecycle.cpp
    src/ui/mainWindow/mainwindow_autoselector.cpp
    src/ui/mainWindow/mainwindow_scanner.cpp
    src/ui/mainWindow/TestRunner.cpp
    src/ui/widget/StartStopButton.cpp
    src/ui/widget/TrayIcon.cpp
    src/ui/widget/TrayPopupFrame.cpp
    src/ui/widget/TrayProfileSelector.cpp
    src/ui/widget/TrayOtpCodes.cpp
    src/ui/widget/SubscriptionInfoCard.cpp
    src/ui/widget/json/JsonTree.cpp
    src/ui/widget/json/JsonCodeEdit.cpp
    src/ui/widget/json/JsonValidator.cpp
    src/ui/widget/json/JsonSchemaValidator.cpp
    src/ui/widget/json/JsonIssueList.cpp
    src/ui/widget/json/JsonEditorDialog.cpp
    src/ui/widget/json/SchemaStore.cpp
    src/ui/profile/dialog_edit_profile.cpp
    src/ui/profile/dialog_edit_profile_singbox.cpp
    src/ui/profile/dialog_edit_profile_xhttp.cpp
    src/ui/profile/dialog_edit_profile_xray.cpp
    src/ui/profile/dialog_vpn_auth.cpp
    src/ui/profile/edit_chain.cpp
    src/ui/profile/edit_advanced.cpp
    src/ui/profile/edit_anytls.cpp
    src/ui/profile/edit_autoselector.cpp
    src/ui/profile/edit_custom.cpp
    src/ui/profile/edit_direct.cpp
    src/ui/profile/edit_extra_core.cpp
    src/ui/profile/edit_http.cpp
    src/ui/profile/edit_hysteria.cpp
    src/ui/profile/edit_hysteria_realm.cpp
    src/ui/profile/edit_juicity.cpp
    src/ui/profile/edit_masque.cpp
    src/ui/profile/edit_mieru.cpp
    src/ui/profile/edit_naive.cpp
    src/ui/profile/edit_openconnect.cpp
    src/ui/profile/edit_openconnect_advanced.cpp
    src/ui/profile/edit_openvpn.cpp
    src/ui/profile/edit_openvpn_advanced.cpp
    src/ui/profile/edit_shadowsocks.cpp
    src/ui/profile/edit_shadowtls.cpp
    src/ui/profile/edit_snell.cpp
    src/ui/profile/edit_socks.cpp
    src/ui/profile/edit_ssh.cpp
    src/ui/profile/edit_tailscale.cpp
    src/ui/profile/edit_trojan.cpp
    src/ui/profile/edit_trusttunnel.cpp
    src/ui/profile/edit_tuic.cpp
    src/ui/profile/edit_vless.cpp
    src/ui/profile/edit_vmess.cpp
    src/ui/profile/edit_wireguard.cpp
    src/ui/profile/edit_wireguard_amnezia.cpp
    src/ui/profile/edit_xrayvless.cpp
    src/ui/profile/ProxyItem.cpp
    src/ui/group/GroupItem.cpp
    src/ui/group/dialog_edit_group.cpp
    src/ui/group/dialog_edit_group_advanced.cpp
    src/ui/group/dialog_manage_groups.cpp
    src/ui/scanner/dialog_scanner.cpp
    src/ui/scanner/dialog_new_scan.cpp
    src/ui/scanner/dialog_ip_lists.cpp
    src/ui/scanner/dialog_edit_ip_list.cpp
    src/ui/scanner/dialog_scan_instance.cpp
    src/ui/scanner/ScanItem.cpp
    src/ui/scanner/IpEntriesModel.cpp
    src/ui/scanner/IpListItem.cpp
    src/ui/setting/dialog_basic_settings.cpp
    src/ui/setting/DiagnosticsTab.cpp
    src/ui/setting/dialog_edit_otp.cpp
    src/ui/setting/dialog_integration.cpp
    src/ui/setting/dialog_manage_routes.cpp
    src/ui/setting/dialog_otp_manager.cpp
    src/ui/setting/dialog_preset_settings.cpp
    src/ui/setting/dialog_vpn_settings.cpp
    src/ui/setting/ThemeManager.cpp
    src/ui/setting/Icon.cpp
    src/ui/setting/OtpItem.cpp
    src/ui/setting/OtpListWidget.cpp
    src/ui/setting/RawRouteItem.cpp
    src/ui/setting/RouteItem.cpp
    src/ui/stats/dialog_auto_selector.cpp
    src/ui/stats/dialog_endpoint_details.cpp
    src/ui/stats/dialog_traffic_stats.cpp
    src/ui/stats/MiniChartWidget.cpp
    src/ui/stats/RuntimeStatsWidget.cpp
    src/ui/stats/TrafficChartWidget.cpp
    src/ui/utils/ConnectionsTreeModel.cpp
    src/ui/utils/ConnectionsTreeFilterProxyModel.cpp
    src/ui/utils/ConnectionsFilterHeader.cpp
    src/ui/utils/DataViewHtmlGenerator.cpp
    src/ui/utils/ProfilesTableModel.cpp
    src/ui/utils/ProfilesFilterProxyModel.cpp
    src/ui/utils/ProfilesTableView.cpp
    src/ui/utils/ProfilesTableVerticalHeader.cpp
    src/ui/utils/ProfilesTableFilterHeader.cpp
    src/ui/utils/ReorderListWidget.cpp
    src/ui/utils/ScreenQrScanner.cpp
)

# 3rdparty
set(THIRDPARTY_SOURCES
    3rdparty/qrcodegen.cpp
    3rdparty/QtExtKeySequenceEdit.cpp
    3rdparty/QrDecoder.cpp
    3rdparty/WinCommander.cpp
    3rdparty/quirc/decode.c
    3rdparty/quirc/identify.c
    3rdparty/quirc/quirc.c
    3rdparty/quirc/version_db.c
    3rdparty/monocypher/monocypher.c
    3rdparty/qv2ray/v2/ui/LogHighlighter.cpp
    3rdparty/qv2ray/v2/ui/QvAutoCompleteTextEdit.cpp
    3rdparty/qv2ray/v2/ui/widgets/speedchart/SpeedWidget.cpp
    # SQLiteCpp sources (compiled directly to avoid submodule dependency on sqlite3)
    3rdparty/SQLiteCpp/src/Backup.cpp
    3rdparty/SQLiteCpp/src/Column.cpp
    3rdparty/SQLiteCpp/src/Database.cpp
    3rdparty/SQLiteCpp/src/Exception.cpp
    3rdparty/SQLiteCpp/src/Savepoint.cpp
    3rdparty/SQLiteCpp/src/sqlite3.c
    3rdparty/SQLiteCpp/src/Statement.cpp
    3rdparty/SQLiteCpp/src/Transaction.cpp
)

# Main executable
qt_add_executable(CoreDeck
    MANUAL_FINALIZATION
    main.cpp
    ${ENTITY_SOURCES}
    ${REPO_SOURCES}
    ${GLOBAL_SOURCES}
    ${INSTANCE_SOURCES}
    ${CONFIG_SOURCES}
    ${API_SOURCES}
    ${SCANNER_SOURCES}
    ${STATS_SOURCES}
    ${SYS_SOURCES}
    ${UI_SOURCES}
    ${THIRDPARTY_SOURCES}
    ${COREDECK_QRC}
    ${PROTO_SRCS}
    ${PROTO_HDRS}
)

# Qt setup
set_target_properties(CoreDeck PROPERTIES
    AUTOUIC ON
    AUTOMOC ON
    AUTORCC ON
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)

if (WIN32 AND MSVC)
    set_target_properties(CoreDeck PROPERTIES UNITY_BUILD ON)
endif ()

set_property(SOURCE "3rdparty/SQLiteCpp/src/sqlite3.c" PROPERTY SKIP_UNITY_BUILD_INCLUSION ON)
set_property(SOURCE "3rdparty/monocypher/monocypher.c" PROPERTY SKIP_UNITY_BUILD_INCLUSION ON)

target_compile_definitions(CoreDeck PRIVATE
    SQLITE_DQS=0
    SQLITE_DEFAULT_MEMSTATUS=0
    SQLITE_DEFAULT_WAL_SYNCHRONOUS=1
    SQLITE_LIKE_DOESNT_MATCH_BLOBS
    SQLITE_MAX_EXPR_DEPTH=0
    SQLITE_UNTESTABLE
    SQLITE_OMIT_DEPRECATED
    SQLITE_OMIT_LOAD_EXTENSION
    SQLITE_THREADSAFE=1
    SQLITE_USE_ALLOCA
)

target_link_libraries(CoreDeck PRIVATE
    Qt6::Widgets
    Qt6::Network
    Qt6::LinguistTools
    ${QT_DBUS_LIBRARY}
    SQLiteCpp
    spb-proto
    protobuf::libprotobuf
    Threads::Threads
    ${CMAKE_DL_LIBS}
)

if (WIN32)
    target_link_libraries(CoreDeck PRIVATE
        version
        winmm
        ws2_32
        iphlpapi
        userenv
        shell32
        ole32
        oleaut32
        uuid
        advapi32
    )
endif ()

if (UNIX AND NOT APPLE)
    target_link_libraries(CoreDeck PRIVATE
        pthread
        dl
        rt
    )
endif ()

# Install
install(TARGETS CoreDeck
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)