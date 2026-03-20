cmake_minimum_required(VERSION 2.8)

project(log)

set(LOG_BACKEND "console" CACHE STRING "Option for specify log backend, supported are: console, syslog")

MESSAGE(LOG_BACKEND=${LOG_BACKEND})

SET(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")

SET(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")

add_library(${PROJECT_NAME} STATIC
    "log.hpp"
    "log.cpp"
    "base.hpp"
    "base.cpp"
    "${LOG_BACKEND}.cpp"
    )

INSTALL (
    TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
    COMPONENT dev
)

INSTALL ( FILES "log.hpp" "base.hpp" DESTINATION "${INSTALL_INCLUDE_DIR}/log" COMPONENT dev )
