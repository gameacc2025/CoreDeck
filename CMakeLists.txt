cmake_minimum_required(VERSION 3.20)

project(CoreDeck VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    string(REPLACE "/Zi" "/Z7" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
endif()

# Dead code stripping
if (MSVC)
    add_link_options(
        $<$<NOT:$<CONFIG:Debug>>:/INCREMENTAL:NO>
        $<$<NOT:$<CONFIG:Debug>>:/OPT:REF>
        $<$<NOT:$<CONFIG:Debug>>:/OPT:ICF>
    )
else ()
    add_compile_options(-ffunction-sections -fdata-sections)
    if (APPLE)
        add_link_options(-Wl,-dead_strip)
    else ()
        add_link_options(-Wl,--gc-sections)
    endif ()
endif ()

if (UNIX AND NOT APPLE)
    set(QT_DBUS_COMPONENT DBus)
    set(QT_DBUS_LIBRARY Qt6::DBus)
endif ()

# Version and compile definitions (NKR_VERSION, NKR_CPP_DEBUG, NKR_CPP_USE_APPDATA)
include(cmake/nkr.cmake)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network Test ${QT_DBUS_COMPONENT})
find_package(SQLite3 REQUIRED)
find_package(protobuf REQUIRED)

# Protobuf generation via myproto.cmake
include(cmake/myproto.cmake)

# Platform-specific settings
if (APPLE)
    # macOS support dropped (DEC-012)
    message(WARNING "macOS support dropped per DEC-012. Build may fail.")
elseif (WIN32)
    include(cmake/windows/windows.cmake)
else ()
    include(cmake/linux/linux.cmake)
endif ()

if (NOT MSVC)
    add_compile_options(-fno-omit-frame-pointer)
endif()

add_compile_definitions(QT_MESSAGELOGCONTEXT)

# SQLiteCpp configuration - compile definitions for both SQLiteCpp wrapper and sqlite3.c
add_compile_definitions(
    SQLITE_DQS=0
    SQLITE_DEFAULT_MEMSTATUS=0
    SQLITE_DEFAULT_WAL_SYNCHRONOUS=1
    SQLITE_LIKE_DOESNT_MATCH_BLOBS
    SQLITE_MAX_EXPR_DEPTH=0
    SQLITE_OMIT_DEPRECATED
    SQLITE_OMIT_LOAD_EXTENSION
    SQLITE_THREADSAFE=1
    SQLITE_USE_ALLOCA
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/SQLiteCpp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/simple-protobuf
    ${CMAKE_CURRENT_BINARY_DIR}
)

# 3rdparty: simple-protobuf (SQLiteCpp sources added directly in src/CMakeLists.txt)
# Handled by cmake/myproto.cmake

# CoreDeck source
add_subdirectory(src)

# Tests
enable_testing()
add_subdirectory(tests)

# Resources
file(GLOB_RECURSE RESOURCES_QRC RELATIVE ${CMAKE_SOURCE_DIR}/res ${CMAKE_SOURCE_DIR}/res/*.qrc)
if (RESOURCES_QRC)
    foreach (QRC_FILE ${RESOURCES_QRC})
        qt6_add_resources(COREDECK_RESOURCES ${QRC_FILE})
    endforeach ()
endif()

# Installation (optional)
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path" FORCE)
endif()