# CoreDeck tests

enable_testing()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/3rdparty/SQLiteCpp/include
    ${CMAKE_SOURCE_DIR}/3rdparty/simple-protobuf
    ${CMAKE_BINARY_DIR}
    ${PROTOBUF_INCLUDE_DIRS}
)

# SQLiteCpp sources for tests (compiled directly to avoid submodule dependency on sqlite3)
set(SQLITECPP_TEST_SOURCES
    ../3rdparty/SQLiteCpp/src/Backup.cpp
    ../3rdparty/SQLiteCpp/src/Column.cpp
    ../3rdparty/SQLiteCpp/src/Database.cpp
    ../3rdparty/SQLiteCpp/src/Exception.cpp
    ../3rdparty/SQLiteCpp/src/Savepoint.cpp
    ../3rdparty/SQLiteCpp/src/sqlite3.c
    ../3rdparty/SQLiteCpp/src/Statement.cpp
    ../3rdparty/SQLiteCpp/src/Transaction.cpp
)

# Platform-specific sys sources for tests
if (WIN32)
    set(SYS_TEST_SOURCES
        ../src/sys/windows/UrlScheme.cpp
        ../src/sys/windows/WinVersion.cpp
        ../src/sys/windows/guihelper.cpp
    )
else ()
    set(SYS_TEST_SOURCES
        ../src/sys/linux/UrlScheme.cpp
    )
endif ()

# Test: Instance entity
add_executable(test_instance
    test_instance.cpp
    ../src/database/entities/Instance.cpp
)
target_link_libraries(test_instance PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Test
)
add_test(NAME test_instance COMMAND test_instance)

# Test: InstanceManager
add_executable(test_instancemanager
    test_instancemanager.cpp
    ../src/instance/InstanceManager.cpp
    ../src/database/InstanceRepository.cpp
    ../src/database/entities/Instance.cpp
    ../src/database/Database.cpp
    ../src/database/SettingsRepo.cpp
    ../src/global/Logger.cpp
    ../src/global/Utils.cpp
    ${SQLITECPP_TEST_SOURCES}
    ${SYS_TEST_SOURCES}
)
target_link_libraries(test_instancemanager PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    Qt6::Test
    spb-proto
    Threads::Threads
)

target_compile_definitions(test_instancemanager PRIVATE
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

if (WIN32)
    target_link_libraries(test_instancemanager PRIVATE
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

add_test(NAME test_instancemanager COMMAND test_instancemanager)

# TODO: Add more tests as they are created
# Migration test (requires database)
# Repository tests
# Integration tests