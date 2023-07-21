cmake_minimum_required(VERSION 3.19)
project(roguelite LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(${CMAKE_SOURCE_DIR}/scripts/cmake/utils.cmake)
include(FetchContent)

FetchContent_Declare(
    GDExtension
    GIT_REPOSITORY https://github.com/godotengine/godot-cpp.git
    GIT_TAG godot-4.1-stable
)

FetchContent_MakeAvailable(GDExtension)

# Automatically pick up files added to src
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# https://bytefreaks.net/programming-2/manually-set-the-cmake-output-folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/bin)

# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/bin)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/bin)
set(CMAKE_DEBUG_POSTFIX "d")

# Add a dynamic library named ${PROJECT_NAME}
add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME} PUBLIC godot::cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX src FILES ${SOURCES})

# =======================================
# CMAKE configuration diagnostics
# =======================================
# prints dependency tree if the option is set
if(ROGUELITE_PRINT_DEPENDENCY_TREE MATCHES ON)
    set_property(GLOBAL PROPERTY GLOBAL_DEPENDS_DEBUG_MODE ON)
endif()

# output cmake variables if option is set
if(ROGUELITE_PRINT_CMAKE_VARIABLES MATCHES ON)
    dump_cmake_variables(".*")
endif()

# print cmake project configuration variables
print_project_variables()
