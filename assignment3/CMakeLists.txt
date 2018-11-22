set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
cmake_minimum_required(VERSION 3.12)
project(assinment3)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../include)

find_library(carbon_lib Carbon) # we look for the Carbon framework and use carbon_lib as an alias for it
find_library(iokit_lib IOKit)
find_library(forcefeedback_lib ForceFeedback)
find_library(cocoa_lib Cocoa)
find_library(corevideo_lib CoreVideo)
find_library(opengl_lib OpenGL)
find_library(corefoundation_lib CoreFoundation)

set(frameworks
        ${carbon_lib}
        ${iokit_lib}
        ${forcefeedback_lib}
        ${cocoa_lib}
        ${corevideo_lib}
        ${opengl_lib}
        ${corefoundation_lib})

find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

FIND_PATH( SOIL_INCLUDE_DIRS SOIL.h PATH_SUFFIXES SOIL)
find_library(SOIL_LIBRARIES SOIL)
include_directories(${SOIL_INCLUDE_DIRS})


add_executable(assinment3 src/main.cpp ../include/glad.c)
target_link_libraries(assinment3 glfw ${frameworks} ${SOIL_LIBRARIES})
# auto copy the glsl&obj files
add_custom_command(TARGET assinment3 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/config $<TARGET_FILE_DIR:assinment3>)
