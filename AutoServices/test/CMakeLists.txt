cmake_minimum_required(VERSION 3.0)

option(INSTALL_GTEST "option for install google test" OFF)

# add google test
set(GTest_SOURCE_DIR ../../../CA_Cockpik_IC_Script/third_party/zl-linux-googletest)
enable_testing()
add_subdirectory(${GTest_SOURCE_DIR} googletest)
include_directories(${GTest_SOURCE_DIR}/googletest/include)
set(GTEST_LIB gtest gtest_main)

add_subdirectory(autosoundservice)