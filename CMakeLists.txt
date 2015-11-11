cmake_minimum_required(VERSION 3.3)
set(PROJECT_NAME tutorial_camera)

# Set the version
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)

project(${PROJECT_NAME})

# Configure header to pass some settings to source code
configure_file(
        "${PROJECT_SOURCE_DIR}/src/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/config.h.in)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

pkg_search_module(GLEW REQUIRED glew)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

message(${CMAKE_SOURCE_DIR}/renderprogs)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/renderprogs $<TARGET_FILE_DIR:${PROJECT_NAME}>/renderprogs)