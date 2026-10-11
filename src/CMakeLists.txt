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
    database/entities/Instance.cpp
    database/entities/Group.cpp
    database/entities/Profile.cpp
    database/entities/RouteProfile.cpp
    database/entities/RouteRule.cpp
    database/entities/OtpProfile.cpp
    database/entities/IpScan.cpp
    database/entities/EndpointSource.cpp
)

# Database repositories
set(REPO_SOURCES
    database/Database.cpp
    database/DatabaseManager.cpp
    database/Migration.cpp
    database/InstanceRepository.cpp
    database/GroupsRepo.cpp
    database/ProfilesRepo.cpp
    database/RoutesRepo.cpp
    database/SettingsRepo.cpp
    database/TrafficStatsRepo.cpp
    database/MarkersRepo.cpp
    database/OtpProfilesRepo.cpp
    database/IpListsRepo.cpp
    database/IpScansRepo.cpp
)

# Global utilities
set(GLOBAL_SOURCES
    global/Configs.cpp
    global/Utils.cpp
    global/Logger.cpp
    global/PeriodicRunner.cpp
    global/HTTPRequestHelper.cpp
    global/DeviceDetailsHelper.cpp
    global/CountryHelper.cpp
    global/LocalNetwork.cpp
    global/OTP.cpp
    global/OtpPlaceholder.cpp
    global/VpnCredentialOverride.cpp
    global/RunningProfiles.cpp
    global/Common.cpp
)

# Instance management
set(INSTANCE_SOURCES
    instance/InstanceManager.cpp
)

# Configs
set(CONFIG_SOURCES
    configs/AutoSelectorPlan.cpp
    configs/common/DialFields.cpp
    configs/common/QUICFields.cpp
    configs/common/TLS.cpp
    configs/common/multiplex.cpp
    configs/common/transport.cpp
    configs/common/Outbound.cpp
    configs/common/OutboundFactory.cpp
    configs/common/utils.cpp
    configs/common/xrayMultiplex.cpp
    configs/common/xrayStreamSetting.cpp
    configs/outbounds/anyTLS.cpp
    configs/outbounds/autoselector.cpp
    configs/outbounds/http.cpp
    configs/outbounds/hysteria.cpp
    configs/outbounds/shadowsocks.cpp
    configs/outbounds/socks.cpp
    configs/outbounds/ssh.cpp
    configs/outbounds/tailscale.cpp
    configs/outbounds/trojan.cpp
    configs/outbounds/tuic.cpp
    configs/outbounds/trusttunnel.cpp
    configs/outbounds/naive.cpp
    configs/outbounds/shadowtls.cpp
    configs/outbounds/vless.cpp
    configs/outbounds/vmess.cpp
    configs/outbounds/wireguard.cpp
    configs/outbounds/masque.cpp
    configs/outbounds/mieru.cpp
    configs/outbounds/openvpn.cpp
    configs/outbounds/openconnect.cpp
    configs/outbounds/custom.cpp
    configs/generate.cpp
    configs/sub/GroupUpdater.cpp
    configs/sub/SubscriptionParser.cpp
    configs/sub/SubscriptionReconcile.cpp
    configs/sub/RouteUpdater.cpp
    configs/sub/clash.cpp
    configs/sub/vpnFileImport.cpp
    configs/sub/warp.cpp
)

# API
set(API_SOURCES
    api/RPC.cpp
    api/remote/Protocol.cpp
    api/remote/Message.cpp
    api/remote/Server.cpp
    api/remote/Config.cpp
    api/remote/Router.cpp
    api/remote/RouterTests.cpp
)

# Scanner
set(SCANNER_SOURCES
    scanner/DefaultIpLists.cpp
    scanner/IpListParse.cpp
    scanner/IpListUpdater.cpp
    scanner/ScanManager.cpp
    scanner/ScanProfiles.cpp
    scanner/ScanWorker.cpp
)

# Stats
set(STATS_SOURCES
    stats/traffic/TrafficLooper.cpp
    stats/traffic/TrafficStatsManager.cpp
    stats/autoselector/AutoSelectorMonitor.cpp
    stats/connectionLister/connectionLister.cpp
)

# Sys
if (WIN32)
    set(SYS_SOURCES
        sys/Process.cpp
        sys/KillSwitch.cpp
        sys/ProcessMetrics.cpp
        sys/CoreDiagnostics.cpp
        sys/UrlScheme.cpp
        sys/SystemProxy.cpp
        sys/GlobalHotkeys.cpp
        sys/coreDump.cpp
        sys/LinuxCap.cpp
        sys/windows/AutoRun.cpp
        sys/windows/eventHandler.cpp
        sys/windows/GlobalHotkeys.cpp
        sys/windows/guihelper.cpp
        sys/windows/MiniDump.cpp
        sys/windows/SystemProxy.cpp
        sys/windows/UrlScheme.cpp
        sys/windows/WinVersion.cpp
    )
else ()
    set(SYS_SOURCES
        sys/Process.cpp
        sys/KillSwitch.cpp
        sys/ProcessMetrics.cpp
        sys/CoreDiagnostics.cpp
        sys/UrlScheme.cpp
        sys/SystemProxy.cpp
        sys/GlobalHotkeys.cpp
        sys/coreDump.cpp
        sys/LinuxCap.cpp
        sys/linux/AutoRun.cpp
        sys/linux/GlobalHotkeysKde.cpp
        sys/linux/GlobalHotkeysX11.cpp
        sys/linux/SystemProxy.cpp
        sys/linux/UrlScheme.cpp
        sys/linux/systemChecks.cpp
    )
endif ()

# UI
set(UI_SOURCES
    ../include/ui/mainwindow.ui
    ui/mainWindow/mainwindow_setup.cpp
    ui/mainWindow/mainwindow_events.cpp
    ui/mainWindow/mainwindow_groups.cpp
    ui/mainWindow/mainwindow_view.cpp
    ui/mainWindow/mainwindow_connections.cpp
    ui/mainWindow/mainwindow_profiles.cpp
    ui/mainWindow/mainwindow_deeplink.cpp
    ui/mainWindow/mainwindow_system.cpp
    ui/mainWindow/mainwindow_log.cpp
    ui/mainWindow/mainwindow_hotkeys.cpp
    ui/mainWindow/mainwindow_profile_lifecycle.cpp
    ui/mainWindow/mainwindow_autoselector.cpp
    ui/mainWindow/mainwindow_scanner.cpp
    ui/mainWindow/TestRunner.cpp
    ui/widget/StartStopButton.cpp
    ui/widget/TrayIcon.cpp
    ui/widget/TrayPopupFrame.cpp
    ui/widget/TrayProfileSelector.cpp
    ui/widget/SubscriptionInfoCard.cpp
    ui/widget/json/JsonTree.cpp
    ui/widget/json/JsonCodeEdit.cpp
    ui/widget/json/JsonSchemaValidator.cpp
    ui/widget/json/JsonIssueList.cpp
    ui/widget/json/JsonEditorDialog.cpp
    ui/widget/json/SchemaStore.cpp
    ui/profile/dialog_edit_profile.cpp
    ui/profile/dialog_edit_profile_singbox.cpp
    ui/profile/dialog_edit_profile_xhttp.cpp
    ui/profile/dialog_edit_profile_xray.cpp
    ui/profile/dialog_vpn_auth.cpp
    ui/profile/edit_chain.cpp
    ui/profile/edit_advanced.cpp
    ui/profile/edit_anytls.cpp
    ui/profile/edit_autoselector.cpp
    ui/profile/edit_custom.cpp
    ui/profile/edit_direct.cpp
    ui/profile/edit_extra_core.cpp
    ui/profile/edit_http.cpp
    ui/profile/edit_hysteria.cpp
    ui/profile/edit_hysteria_realm.cpp
    ui/profile/edit_juicity.cpp
    ui/profile/edit_masque.cpp
    ui/profile/edit_mieru.cpp
    ui/profile/edit_naive.cpp
    ui/profile/edit_openconnect.cpp
    ui/profile/edit_openconnect_advanced.cpp
    ui/profile/edit_openvpn.cpp
    ui/profile/edit_openvpn_advanced.cpp
    ui/profile/edit_shadowsocks.cpp
    ui/profile/edit_shadowtls.cpp
    ui/profile/edit_snell.cpp
    ui/profile/edit_socks.cpp
    ui/profile/edit_ssh.cpp
    ui/profile/edit_tailscale.cpp
    ui/profile/edit_trojan.cpp
    ui/profile/edit_trusttunnel.cpp
    ui/profile/edit_tuic.cpp
    ui/profile/edit_vless.cpp
    ui/profile/edit_vmess.cpp
    ui/profile/edit_wireguard.cpp
    ui/profile/edit_wireguard_amnezia.cpp
    ui/profile/edit_xrayvless.cpp
    ui/profile/ProxyItem.cpp
    ui/group/GroupItem.cpp
    ui/group/dialog_edit_group.cpp
    ui/group/dialog_edit_group_advanced.cpp
    ui/group/dialog_manage_groups.cpp
    ui/scanner/dialog_scanner.cpp
    ui/scanner/dialog_new_scan.cpp
    ui/scanner/dialog_ip_lists.cpp
    ui/scanner/dialog_edit_ip_list.cpp
    ui/scanner/dialog_scan_instance.cpp
    ui/scanner/ScanItem.cpp
    ui/scanner/IpEntriesModel.cpp
    ui/scanner/IpListItem.cpp
    ui/setting/dialog_basic_settings.cpp
    ui/setting/DiagnosticsTab.cpp
    ui/setting/dialog_edit_otp.cpp
    ui/setting/dialog_integration.cpp
    ui/setting/dialog_manage_routes.cpp
    ui/setting/dialog_otp_manager.cpp
    ui/setting/dialog_preset_settings.cpp
    ui/setting/dialog_vpn_settings.cpp
    ui/setting/ThemeManager.cpp
    ui/setting/Icon.cpp
    ui/setting/OtpItem.cpp
    ui/setting/OtpListWidget.cpp
    ui/setting/RawRouteItem.cpp
    ui/setting/RouteItem.cpp
    ui/stats/dialog_auto_selector.cpp
    ui/stats/dialog_endpoint_details.cpp
    ui/stats/dialog_traffic_stats.cpp
    ui/stats/MiniChartWidget.cpp
    ui/stats/RuntimeStatsWidget.cpp
    ui/stats/TrafficChartWidget.cpp
    ui/utils/ConnectionsTreeModel.cpp
    ui/utils/ConnectionsTreeFilterProxyModel.cpp
    ui/utils/DataViewHtmlGenerator.cpp
    ui/utils/ProfilesTableModel.cpp
    ui/utils/ProfilesFilterProxyModel.cpp
    ui/utils/ProfilesTableView.cpp
    ui/utils/ProfilesTableVerticalHeader.cpp
    ui/utils/ReorderListWidget.cpp
    ui/utils/ScreenQrScanner.cpp
)

# 3rdparty
set(THIRDPARTY_SOURCES
    ../3rdparty/qrcodegen.cpp
    ../3rdparty/QtExtKeySequenceEdit.cpp
    ../3rdparty/QrDecoder.cpp
    ../3rdparty/WinCommander.cpp
    ../3rdparty/quirc/decode.c
    ../3rdparty/quirc/identify.c
    ../3rdparty/quirc/quirc.c
    ../3rdparty/quirc/version_db.c
    ../3rdparty/monocypher/monocypher.c
    ../3rdparty/qv2ray/v2/ui/LogHighlighter.cpp
    ../3rdparty/qv2ray/v2/ui/QvAutoCompleteTextEdit.cpp
    ../3rdparty/qv2ray/v2/ui/widgets/speedchart/SpeedWidget.cpp
    # SQLiteCpp sources (compiled directly to avoid submodule dependency on sqlite3)
    ../3rdparty/SQLiteCpp/src/Backup.cpp
    ../3rdparty/SQLiteCpp/src/Column.cpp
    ../3rdparty/SQLiteCpp/src/Database.cpp
    ../3rdparty/SQLiteCpp/src/Exception.cpp
    ../3rdparty/SQLiteCpp/src/Savepoint.cpp
    ../3rdparty/SQLiteCpp/src/sqlite3.c
    ../3rdparty/SQLiteCpp/src/Statement.cpp
    ../3rdparty/SQLiteCpp/src/Transaction.cpp
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

set_property(SOURCE "../3rdparty/SQLiteCpp/src/sqlite3.c" PROPERTY SKIP_UNITY_BUILD_INCLUSION ON)
set_property(SOURCE "../3rdparty/monocypher/monocypher.c" PROPERTY SKIP_UNITY_BUILD_INCLUSION ON)

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
    ${QT_DBUS_LIBRARY}
    SQLiteCpp
    spb-proto
    ${PROTOBUF_LIBRARIES}
    Threads::Threads
    ${CMAKE_DL_LIBS}
)

target_include_directories(CoreDeck PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/3rdparty
    ${CMAKE_SOURCE_DIR}/3rdparty/SQLiteCpp/include
    ${CMAKE_SOURCE_DIR}/3rdparty/simple-protobuf
    ${CMAKE_SOURCE_DIR}/3rdparty/simple-protobuf/include
    ${CMAKE_BINARY_DIR}
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