cmake_minimum_required(VERSION 3.20)

## Uncomment to output a dump of every CMake variable
# set(CMAKE_MESSAGE_LOG_LEVEL DEBUG)

# =======================================================================
# VCPKG submodule init/update. Needs to happen before anything else
# because this submodule contains a toolchain file needed for the 
# main project configuration, which must set before project() is called
# =======================================================================

# confirm we found the vcpkg submodule ports dir.
# if the sources list is empty, the submodule probably
# hasn't been initialized or updated yet.
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/vcpkg/ports")
    message(NOTICE "VCPKG package manager sources not found")
    message(NOTICE "initializing/updating the vcpkg submodule...")

    # update the vcpkg submodule to populate it with the code necessary
    # to grab all dependencies needed for the gdextension library build
    execute_process(
        COMMAND git submodule update --init extern/vcpkg
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

# define toolchain file early for vcpkg. only set the variable if the file exists.
# if this is the first configuration after a clean clone of this repo, this file
# won't exist until the submodule is initialized below. configuration will fail if this
# is set to a path that doesn't exist, so only set it after the first pass of configuration
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(toolchain_file_path "${CMAKE_CURRENT_SOURCE_DIR}/extern/vcpkg/scripts/buildsystems/vcpkg.cmake")
    if (EXISTS "${toolchain_file_path}")
        set(CMAKE_TOOLCHAIN_FILE "${toolchain_file_path}")
    else()
        message(WARNING "VCPKG toolchain file not found: ${toolchain_file_path}")
    endif()
endif()

project(roguelite LANGUAGES C CXX)
set(gdextension_lib_name ${PROJECT_NAME})

string(TOLOWER "${CMAKE_SYSTEM_NAME}" host_os)
set(VCPKG_TARGET_TRIPLET "x64-${host_os}-static")

# needs to be >= C++17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

# define output paths for gdextension shared lib and debug symbols
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/project/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/project/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/project/bin")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/project/bin")
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/project/bin")

# postfix debug binaries with "d"
set(CMAKE_DEBUG_POSTFIX "d")

# confirm we found the godot engine source files.
# if the sources list is empty, the submodule probably
# hasn't been initialized or updated yet.
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/core")
    message(NOTICE "Godot engine sources not found")
    message(NOTICE "initializing/updating the engine submodule...")

    # update the engine submodule to populate it with the
    # code necessary to build a debug version of the editor that
    # can be easily debugged along with the gdextension library
    execute_process(
        COMMAND git submodule update --init extern/godot-engine
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

# =======================================================================
# VCPKG bootstrap / initialization.
# Only happens once, when vcpkg executable is missing.
# =======================================================================

set(vcpkg_executable "${CMAKE_CURRENT_SOURCE_DIR}/extern/vcpkg/vcpkg${CMAKE_EXECUTABLE_SUFFIX}")

if(EXISTS "${vcpkg_executable}")
    message(NOTICE "Found VCPKG Executable: ${vcpkg_executable}")
else()
    message(NOTICE "Could not find VCPKG Executable: ${vcpkg_executable}")
    message(NOTICE "Calling VCPKG bootstrap scripts.")
    # bootstrap vcpkg to configured and install

    if(WIN32)
        execute_process(
            COMMAND ps -c "${CMAKE_CURRENT_SOURCE_DIR}/extern/vcpkg/bootstrap-vcpkg.bat"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            COMMAND_ERROR_IS_FATAL ANY
        )
    elseif(UNIX)
        execute_process(
            COMMAND bash "${CMAKE_CURRENT_SOURCE_DIR}/extern/vcpkg/bootstrap-vcpkg.sh"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            COMMAND_ERROR_IS_FATAL ANY
        )
    endif()

    # fail out if vcpkg isn't found after setup
    if(NOT EXISTS "${vcpkg_executable}")
        message(FATAL_ERROR "ERROR: '${vcpkg_executable}' not found!")
    endif()
endif()

# =======================================================================
# Godot Engine and C++ bindings submodule management
# =======================================================================

# confirm we found the godot engine source files.
# if the sources list is empty, the submodule probably
# hasn't been initialized or updated yet.
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-cpp/src")
    message(NOTICE "godot-cpp bingings source not found")
    message(NOTICE "initializing/updating the godot-cpp submodule...")

    # update the c++ bingings submodule to populate it with
    # the necessary source for the gdextension library
    execute_process(
        COMMAND git submodule update --init extern/godot-cpp
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

# =======================================================================
# Godot editor/engine debug build
# =======================================================================

set(host_os_engine "${host_os}")
if(UNIX)
    set(host_os_engine "${host_os}bsd")
endif()

set(godot_debug_editor_executable
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/bin/godot.${host_os_engine}.editor.dev.x86_64${CMAKE_EXECUTABLE_SUFFIX}"
)

message(NOTICE "godot_debug_editor_executable = ${godot_debug_editor_executable}")

if(NOT EXISTS "${godot_debug_editor_executable}")
    message("Godot engine debug binaries not found, invoking debug build of engine...")

    execute_process(
        COMMAND scons platform=${host_os} arch=x64 target=editor use_static_cpp=yes dev_build=yes debug_symbols=yes optimize=none use_lto=no --clean
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine"
        COMMAND_ERROR_IS_FATAL ANY
    )

    # this build should only ever need to be run once (unless the enging debug binaries
    # are deleted or you want to change the build configuration/command invoked below).
    execute_process(
        COMMAND scons platform=${host_os} arch=x64 target=editor use_static_cpp=yes dev_build=yes debug_symbols=yes optimize=none use_lto=no use_lto=no
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine"
        COMMAND_ERROR_IS_FATAL ANY
    )

    # not necessary, the temp file in here just confuses Visual Studio
    file(REMOVE_RECURSE "${CMAKE_CURRENT_SOURCE_DIR}}/extern/godot-engine/.sconf_temp")

    if(NOT EXISTS "${godot_debug_editor_executable}")
        message(FATAL_ERROR "Couldn't find godot debug executable after scons build: ${godot_debug_editor_executable}")
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

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-cpp)

# =======================================================================
# Godot engine library setup/configuration.
# Not necessary, just provides better support in multiple IDEs
# for engine source code browsing, intellisense, and debugging
# =======================================================================

# populate source file list for the godot engine submodule
file(GLOB_RECURSE godot_engine_sources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/*.[hc]"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/*.[hc]pp"
)

# add the engine sources as a library so intellisense actually works
add_library(godot_engine EXCLUDE_FROM_ALL ${godot_engine_sources})
target_include_directories(godot_engine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/platform/windows"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/thirdparty/zlib"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/thirdparty/vulkan/include"
    SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/thirdparty/zstd"
    SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/thirdparty/mbedtls/include"
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

set(compiler_is_clang "$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>>")
set(compiler_is_gnu "$<CXX_COMPILER_ID:GNU>")
set(compiler_is_msvc "$<CXX_COMPILER_ID:MSVC>")

include("${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-cpp/cmake/GodotCompilerWarnings.cmake")

# set compiler options for the gdextension library
target_compile_options(${gdextension_lib_name} PUBLIC
    $<${compiler_is_msvc}:
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
    $<$<NOT:${compiler_is_msvc}>:
        -g
        -Wno-unused-value
        $<${compiler_is_gnu}:
            -Wno-attributes=rl::
        >
        $<${compiler_is_clang}:
            -Wno-unknown-attributes
        >
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
    $<$<NOT:${compiler_is_msvc}>:
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
    PRIVATE spdlog::spdlog_header_only
)

# =======================================================================
# Print configuration report
# =======================================================================

include(${CMAKE_SOURCE_DIR}/scripts/cmake/utils.cmake)
print_project_variables()
