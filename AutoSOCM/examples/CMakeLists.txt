cmake_minimum_required(VERSION 2.8)

INCLUDE_DIRECTORIES("../include")

add_executable(example_client "example_client.cpp")

target_link_libraries(example_client ${PROJECT_NAME} pthread ${Boost_LIBRARIES})

add_executable(example_service "example_service.cpp")

target_link_libraries(example_service ${PROJECT_NAME} pthread ${Boost_LIBRARIES})
