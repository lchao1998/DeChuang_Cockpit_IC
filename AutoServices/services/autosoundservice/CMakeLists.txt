cmake_minimum_required (VERSION 3.0)

project (autosoundservice)

set(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE(appfw   COMPONENTS thread REQUIRED)
FIND_PACKAGE(autosocm COMPONENTS thread REQUIRED)

FILE (GLOB_RECURSE AUTOSOUNDSERVICE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR} "include/*.h*" )
FILE (GLOB_RECURSE AUTOSOUNDSERVICE_AUDIOS  ${CMAKE_CURRENT_SOURCE_DIR} "resources/*.wav" )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/soundmanager       ipc_sound_manager_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/soundservices      ipc_service_sound_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/player             ipc_player_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/resourcemanager    ipc_resource_manager_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/controller         ipc_controller_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/soundclient        ipc_sound_client_src)

set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
set(INSTALL_RESOURCES_DIR /usr/local/audios)
set(ipc_source auto_sound_service_app.cpp)

add_executable(autosoundservice
        ${ipc_source}
        ${ipc_sound_manager_src}
        ${ipc_service_sound_src}
        ${ipc_player_src}
        ${ipc_resource_manager_src}
        ${ipc_sound_client_src}
        ${ipc_controller_src}
        )

target_link_libraries(autosoundservice pthread asound sndfile rt log ${AUTOSOCM_LIBRARIES} ${APPFW_LIBRARIES})

# INSTALL
foreach ( file ${AUTOSOUNDSERVICE_HEADERS} )
    install( FILES "${file}" DESTINATION "${INSTALL_INCLUDE_DIR}/autosoundservice" COMPONENT dev)
endforeach()

foreach ( file ${AUTOSOUNDSERVICE_AUDIOS} )
    install( FILES "${file}" DESTINATION "${INSTALL_RESOURCES_DIR}" COMPONENT dev)
endforeach()

install(TARGETS autosoundservice DESTINATION ${INSTALL_BIN_DIR})

add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
        TARGET "uninstall"
        POST_BUILD
        COMMENT "Uninstall files with install_manifest.txt"
        COMMAND xargs rm -vf < ${PROJECT_BINARY_DIR}/../../install_manifest.txt || echo Nothing in
        install_manifest.txt to be uninstalled!
)
