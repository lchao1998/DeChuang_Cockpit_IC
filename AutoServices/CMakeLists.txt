cmake_minimum_required(VERSION 3.0)

project(autoservices)

# add_definitions(-D MAKE_X86_TARGET)

option(ENABLE_IPCLADAPTOR 	"option for build ipcl adaptor" ON)
option(ENABLE_SOUNDMANAGER 	"option for build sound manager" ON)
option(ENABLE_TEST	 	"option for build test case" OFF)

SET(INSTALL_BIN_DIR         bin/autoservices    CACHE PATH "Executable install dir")
SET(INSTALL_LIB_DIR         lib					CACHE PATH "Library install dir")
SET(INSTALL_INCLUDE_DIR     include				CACHE PATH "Include install dir")

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#TODO: remove common headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tools)

if (ENABLE_IPCLADAPTOR)
    add_subdirectory(services/autodataservice)
endif()

if (ENABLE_SOUNDMANAGER)
    #add_subdirectory(services/autosoundservice)
endif()

if (ENABLE_TEST)
    add_subdirectory(test)
endif()
