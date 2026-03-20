cmake_minimum_required (VERSION 3.0)

project (autodataservice)

FIND_PACKAGE(appfw   COMPONENTS thread REQUIRED)
FIND_PACKAGE(autosocm COMPONENTS thread REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../tools/socketcan)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/servicemanager)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dataservices)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../../tools/socketcan              tools_socketcan_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/dataservices                       ipc_service_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/dataservices/domainservice         ipc_service_domain_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/dataservices/gaugeservice          ipc_service_gauge_src)

set(ipcl_source
	auto_data_service_app.cpp
    ./servicemanager/service_manager.cpp
	)

add_executable(autodataservice
                ${tools_socketcan_src}
                ${ipcl_source}
                ${ipc_service_src}
                ${ipc_service_gauge_src}
                ${ipc_service_domain_src}
                )

target_link_libraries(autodataservice pthread rt ${AUTOSOCM_LIBRARIES} ${APPFW_LIBRARIES} log)

INSTALL(TARGETS autodataservice DESTINATION ${INSTALL_BIN_DIR})
