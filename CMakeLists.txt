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


#if (UNIX)
#    find_package(PkgConfig REQUIRED)
#    pkg_search_module(GLFW REQUIRED glfw3)
#    include_directories(${GLFW_INCLUDE_DIRS})
#
#    pkg_search_module(GLEW REQUIRED glew)
#    include_directories(${GLEW_INCLUDE_DIRS})
#
#    find_package(OpenGL REQUIRED)
#    include_directories(${OPENGL_INCLUDE_DIRS})
#endif (UNIX)

#target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

if (APPLE)
    FIND_LIBRARY(IOKit_LIBRARY IOKit)
    find_package(OpenGL REQUIRED)
    #include_directories(${OPENGL_INCLUDE_DIRS})
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    FIND_LIBRARY(CORE_VIDEO CoreVideo)
endif (APPLE)

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glfw-3.1.2)
include_directories(${CMAKE_SOURCE_DIR}/lib/glfw-3.1.2/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glew)
include_directories(${CMAKE_SOURCE_DIR}/lib/glew/include)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} glew ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${IOKit_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${COCOA_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${CORE_VIDEO})

message(${CMAKE_SOURCE_DIR}/renderprogs)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/renderprogs $<TARGET_FILE_DIR:${PROJECT_NAME}>/renderprogs)