# CoreDeck tests

enable_testing()

# Test: Instance entity
add_executable(test_instance
    tests/test_instance.cpp
    src/database/entities/Instance.cpp
)
target_link_libraries(test_instance PRIVATE
    Qt6::Core
    Qt6::Test
)
add_test(NAME test_instance COMMAND test_instance)

# Test: InstanceManager
add_executable(test_instancemanager
    tests/test_instancemanager.cpp
    src/instance/InstanceManager.cpp
    src/database/InstanceRepository.cpp
    src/database/entities/Instance.cpp
    src/database/Database.cpp
    src/database/SettingsRepo.cpp
    src/global/Logger.cpp
    src/global/Utils.cpp
)
target_link_libraries(test_instancemanager PRIVATE
    Qt6::Core
    Qt6::Test
    SQLiteCpp
    spb-proto
    Threads::Threads
)
add_test(NAME test_instancemanager COMMAND test_instancemanager)

# TODO: Add more tests as they are created
# Migration test (requires database)
# Repository tests
# Integration tests