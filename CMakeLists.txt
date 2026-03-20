cmake_minimum_required(VERSION 3.1)

project(DeChuang_Cockpit_IC)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(Log)
add_subdirectory(appfw)
add_subdirectory(AutoSOCM)
add_subdirectory(AutoServices)
add_subdirectory(HMIControllers)

