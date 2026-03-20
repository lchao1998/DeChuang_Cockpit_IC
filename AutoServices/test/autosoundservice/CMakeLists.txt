project (test_autosoundservice)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(appfw   COMPONENTS thread REQUIRED)
FIND_PACKAGE(autosocm COMPONENTS thread REQUIRED)
FIND_PACKAGE(vsomeip REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../tools)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../services/autosoundservice)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../../services/autosoundservice/player             ipc_player_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../../services/autosoundservice/resourcemanager    ipc_resource_manager_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../../services/autosoundservice/soundclient        ipc_sound_client_src)

add_executable(test_resource_manager
        test_resource_manager.cpp
        ${ipc_resource_manager_src}
        )
target_link_libraries(test_resource_manager ${GTEST_LIB} log)

add_executable(test_player
        test_player.cpp
        ${ipc_player_src}
        ${ipc_resource_manager_src}
        )
target_link_libraries(test_player asound sndfile log ${GTEST_LIB})

add_executable(test_sound_service
        test_sound_service.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../services/autosoundservice/soundservices/service_define.cpp
        ${ipc_resource_manager_src}
        )
target_link_libraries(test_sound_service asound sndfile log ${GTEST_LIB} ${AUTOSOCM_LIBRARIES})

add_executable(test_client
        test_client.cpp
        ${ipc_sound_client_src}
        )
target_link_libraries(test_client log ${VSOMEIP_LIBRARIES} ${GTEST_LIB})

add_custom_command(TARGET test_player POST_BUILD            # Adds a post-build event to MyTest
        COMMAND ${CMAKE_COMMAND} -E copy_directory          # which executes "cmake - E copy_if_different..."
        ${CMAKE_CURRENT_SOURCE_DIR}/resources             # <--this is in-file
        $<TARGET_FILE_DIR:test_player>)

add_test(NAME test_resource_manager COMMAND test_resource_manager)
add_test(NAME test_player           COMMAND test_player)
add_test(NAME test_sound_service    COMMAND test_sound_service)
add_test(NAME test_client           COMMAND test_client)
