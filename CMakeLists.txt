cmake_minimum_required(VERSION 3.28)

set(CMAKE_TOOLCHAIN_FILE "D://vcpkg//scripts//buildsystems//vcpkg.cmake" CACHE STRING "")

project(gus)

set(VERSION 0.0.1)

set(LANGUAGE CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Sql)

qt_standard_project_setup()

qt6_add_resources(QRC_SOURCES "src/assets/assets.qrc")

qt_add_executable(${PROJECT_NAME} ${QRC_SOURCES})

target_sources(${PROJECT_NAME}
    PRIVATE
    "src/main.cpp"
    "src/utils/utils.h"
    "src/view/window.h"
    "src/view/window.cpp"
    "src/view/window.ui"
    "src/components/userItem.h"
    "src/components/addItem.h"
    "src/datastore/user.h"
    "src/datastore/db.h"
    "src/components/addDialog.h"
)

include_directories(
    ${PROJECT_SOURCE_DIR}/third_party/libsass/include
)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LIBSASS_LIB_DIR ${PROJECT_SOURCE_DIR}/third_party/libsass/lib/debug)
    set(LIBSASS_BIN_DIR ${PROJECT_SOURCE_DIR}/third_party/libsass/bin/debug)
else()
    set(LIBSASS_LIB_DIR ${PROJECT_SOURCE_DIR}/third_party/libsass/lib/release)
    set(LIBSASS_BIN_DIR ${PROJECT_SOURCE_DIR}/third_party/libsass/bin/release)
endif()

target_link_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${LIBSASS_LIB_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Sql
    libsass
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${LIBSASS_BIN_DIR}/libsass.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/third_party/openssl/bin $<TARGET_FILE_DIR:${PROJECT_NAME}>
)