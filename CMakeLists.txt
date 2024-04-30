cmake_minimum_required(VERSION 3.28)

project(gus)

set(VERSION 0.0.1)

set(LANGUAGE CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

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
 "src/components/userItem.h" "src/components/addItem.h")

include_directories(
    ${PROJECT_SOURCE_DIR}/third_party/sass/include
)

target_link_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/third_party/sass/lib
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Sass
)