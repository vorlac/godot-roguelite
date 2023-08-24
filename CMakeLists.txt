cmake_minimum_required(VERSION 3.20)
# set(VCPKG_TARGET_TRIPLET "x64-windows-static")

project(roguelite LANGUAGES C CXX)
set(gdextension_lib_name ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# define output paths for gdextension shared lib and debug symbols
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/bin)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/bin)
set(CMAKE_DEBUG_POSTFIX "d")

# =======================================================================
# Godot Engine and C++ bindings submodule management
# =======================================================================

# confirm we found the godot engine source files.
# if the sources list is empty, the submodule probably
# hasn't been initialized or updated yet.
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/godot/godot-cpp/src")
    message(STATUS "godot-cpp bingings source not found")
    message(STATUS "initializing/updating the godot-cpp submodule...")

    # update the c++ bingings submodule to populate it with
    # the necessary source for the gdextension library
    execute_process(
        COMMAND git submodule update --init godot/godot-cpp
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

# confirm we found the godot engine source files.
# if the sources list is empty, the submodule probably
# hasn't been initialized or updated yet.
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/godot/engine/core")
    message("Godot engine sources not found")
    message("initializing/updating the engine submodule...")

    # update the engine submodule to populate it with the
    # code necessary to build a debug version of the editor that
    # can be easily debugged along with the gdextension library
    execute_process(
        COMMAND git submodule update --init godot/engine
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()


# confirm we found the vcpkg submodule ports dir.
# if the sources list is empty, the submodule probably
# hasn't been initialized or updated yet.
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/extern/vcpkg/ports")
    message("VCPKG package manager sources not found")
    message("initializing/updating the vcpkg submodule...")

    # update the vcpkg submodule to populate it with the code necessary
    # to grab all dependencies needed for the gdextension library build
    execute_process(
        COMMAND git submodule update --init tools/vcpkg
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

# =======================================================================
# VCPKG bootstrap / initialization. 
# Only happens once, when vcpkg.exe is missing.
# =======================================================================

set(vcpkg_executable "${CMAKE_SOURCE_DIR}/extern/vcpkg/vcpkg.exe")

if(NOT EXISTS "${vcpkg_executable}")
    # bootstrap vcpkg so vcpkg.exe 
    # is configured and installed
    execute_process(
        COMMAND ps -c "./extern/vcpkg/boostrap-vcpkg.bat"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMAND_ERROR_IS_FATAL ANY
    )

    # fail out if vcpkg isn't found after setup
    if(NOT EXISTS "${vcpkg_executable}")
        message(FATAL_ERROR "ERROR: extern/vcpkg/vcpkg.exe not found!")
    endif()
endif()


# =======================================================================
# Godot editor/engine debug build
# =======================================================================

set(godot_debug_editor_executable "${CMAKE_SOURCE_DIR}/godot/engine/bin/godot.windows.editor.dev.x86_64.exe")
if(NOT EXISTS "${godot_debug_editor_executable}")
    message("Godot engine debug binaries not found, invoking debug build of engine...")

    execute_process(
        COMMAND scons arch=x64            \\
                      platform=windows    \\
                      target=editor       \\
                      use_static_cpp=no   \\
                      dev_build=yes       \\
                      debug_symbols=yes   \\
                      optimize=none       \\
                      use_lto=no          \\
                      --clean                 
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/godot/engine"
        COMMAND_ERROR_IS_FATAL ANY
    )

    # this build should only ever need to be run once (unless the
    # enging debug binaries are deleted or you want to change the
    # build configuration/command invoked below).
    execute_process(
        COMMAND scons arch=x64            \\
                      platform=windows    \\
                      target=editor       \\
                      use_static_cpp=no   \\
                      dev_build=yes       \\
                      debug_symbols=yes   \\
                      optimize=none       \\
                      use_lto=no          \\
                      --quiet                 
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/godot/engine"
        COMMAND_ERROR_IS_FATAL ANY
    )

    file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/godot/engine/.sconf_temp")

    if(NOT EXISTS "${godot_debug_editor_executable}")
        message(FATAL_ERROR "Couldn't find godot debug executable after scons build")
    endif()
endif()

# =======================================================================
# 3rd party library setup/configuration (leverages vcpkg)
# =======================================================================

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# =======================================================================
# Godot C++ bindings library setup/configuration
# =======================================================================

add_subdirectory(${CMAKE_SOURCE_DIR}/godot/godot-cpp)

# =======================================================================
# Godot engine library setup/configuration.
# Not necessary, just provides better support in Visual Studio
# for engine source code browsing, intellisense, and debugging
# =======================================================================

# populate source file list for the godot engine submodule
file(GLOB_RECURSE godot_engine_sources CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/godot/engine/*.[hc]"
    "${CMAKE_SOURCE_DIR}/godot/engine/*.[hc]pp"
)

# add the engine sources as a library so intellisense actually works
add_library(godot_engine EXCLUDE_FROM_ALL ${godot_engine_sources})
target_include_directories(godot_engine PUBLIC
    "${CMAKE_SOURCE_DIR}/godot/engine"
    "${CMAKE_SOURCE_DIR}/godot/engine/platform/windows"
    "${CMAKE_SOURCE_DIR}/godot/engine/thirdparty/zlib"
    "${CMAKE_SOURCE_DIR}/godot/engine/thirdparty/vulkan/include"
    SYSTEM "${CMAKE_SOURCE_DIR}/godot/engine/thirdparty/zstd"
    SYSTEM "${CMAKE_SOURCE_DIR}/godot/engine/thirdparty/mbedtls/include"
)

# define a bunch of the same symbol definitions 
# used when the engine was build using scons
target_compile_definitions(godot_engine PUBLIC
    $<$<CONFIG:Debug>:
        DEBUG_ENABLED
		DEBUG_METHODS_ENABLED
        DEV_ENABLED
    >
    NOMINMAX
    TOOLS_ENABLED
    NO_EDITOR_SPLASH
    WINDOWS_ENABLED
    WASAPI_ENABLED
    WINMIDI_ENABLED
    TYPED_METHOD_BIND
    VULKAN_ENABLED
    GLES3_ENABLED
    MINIZIP_ENABLED
    BROTLI_ENABLED
    ZSTD_STATIC_LINKING_ONLY
    USE_VOLK
    VK_USE_PLATFORM_WIN32_KHR
    GLAD_ENABLED
    GLES_OVER_GL
)

# =======================================================================
# GDExtension dynamic library setup/configuration
# =======================================================================

# create gdextension dynamic lib from the project src
file(GLOB_RECURSE roguelite_sources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[hc]"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[hc]pp"
)

# add the gdextension dynamic library
add_library(${gdextension_lib_name} SHARED ${roguelite_sources})

# set compiler options for the gdextension library
target_compile_options(${gdextension_lib_name} PUBLIC
    $<${MSVC}:
        /EHsc
        /utf-8
        /Zc:preprocessor
        $<$<CONFIG:Debug>:
            /MDd
        >
        $<$<CONFIG:Release>:
            /MD
            /O2
        >
    >
    $<$<NOT:${MSVC}>:
        -g
        /utf-8
        /Zc:preprocessor
        $<$<CONFIG:Debug>:
            -fno-omit-frame-pointer
            -O0
        >
        $<$<CONFIG:Release>:
            -O3
        >
    >
)

target_include_directories(${gdextension_lib_name} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_options(${gdextension_lib_name} PRIVATE
	$<$<NOT:${MSVC}>:
		-static-libgcc
		-static-libstdc++
		-Wl,-R,'$$ORIGIN'
	>
)

# =======================================================================
# Dependency linkage
# =======================================================================

# link gdextension to the cpp bindings library
target_link_libraries(${gdextension_lib_name}
    PUBLIC godot::cpp
    PRIVATE fmt::fmt 
    PRIVATE fmt::fmt-header-only
    PRIVATE spdlog::spdlog
)

# =======================================================================
# Print useful configuration report
# =======================================================================

include(${CMAKE_SOURCE_DIR}/scripts/cmake/utils.cmake)
print_project_variables()
