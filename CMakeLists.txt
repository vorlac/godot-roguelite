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


# # confirm we found the vcpkg submodule ports dir.
# # if the sources list is empty, the submodule probably
# # hasn't been initialized or updated yet.
# if(NOT EXISTS "${CMAKE_SOURCE_DIR}/tools/vcpkg/ports")
#     message("VCPKG package manager sources not found")
#     message("initializing/updating the vcpkg submodule...")
# 
#     # update the vcpkg submodule to populate it with the code necessary
#     # to grab all dependencies needed for the gdextension library build
#     execute_process(
#         COMMAND git submodule update --init tools/vcpkg
#         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#         COMMAND_ERROR_IS_FATAL ANY
#     )
# endif()
#
# # =======================================================================
# # VCPKG bootstrap / initialization. 
# # Only happens once, when vcpkg.exe is missing.
# # =======================================================================
#
# set(vcpkg_executable "${CMAKE_SOURCE_DIR}/tools/vcpkg/vcpkg.exe")
# 
# if(NOT EXISTS "${vcpkg_executable}")
#     # bootstrap vcpkg so vcpkg.exe 
#     # is configured and installed
#     execute_process(
#         COMMAND ps -c "./tools/vcpkg/boostrap-vcpkg.bat"
#         WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
#         COMMAND_ERROR_IS_FATAL ANY
#     )
# 
#     # fail out if vcpkg isn't found after setup
#     if(NOT EXISTS "${vcpkg_executable}")
#         message(FATAL_ERROR "ERROR: tools/vcpkg/vcpkg.exe not found!")
#     endif()
# endif()


# =======================================================================
# Godot editor/engine debug build
# =======================================================================

set(godot_debug_editor_executable "${CMAKE_SOURCE_DIR}/godot/engine/bin/godot.windows.editor.dev.x86_64.exe")
if(NOT EXISTS "${godot_debug_editor_executable}")
    message("Godot engine debug binaries not found, invoking debug build of engine...")

#    execute_process(
#        COMMAND scons arch=x64            \\
#                      platform=windows    \\
#                      target=editor       \\
#                      use_static_cpp=no   \\
#                      dev_build=yes       \\
#                      debug_symbols=yes   \\
#                      optimize=none       \\
#                      use_lto=no          \\
#                      --clean                 
#        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/godot/engine"
#        COMMAND_ERROR_IS_FATAL ANY
#    )

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
    SYSTEM "${CMAKE_SOURCE_DIR}/godot/engine/thirdparty/zstd"
    SYSTEM "${CMAKE_SOURCE_DIR}/godot/engine/thirdparty/mbedtls/include"
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
add_library(${PROJECT_NAME} SHARED ${roguelite_sources})
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# =======================================================================
# Compiler and linker configuration.
# Build flags/definitions match what the engine and bindings use.
# =======================================================================

if(MSVC)
    # using Visual Studio C++

    # set compiler definitions that match up with the buid used for the editor:
    # scons platform=windows arch=x86_64 target=editor dev_build=yes
    # debug_symbols=yes verbose=yes warnings=extra werror=yes
    add_compile_definitions(
        NOMINMAX
        TOOLS_ENABLED
        DEBUG_ENABLED
        DEV_ENABLED
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

    set(roguelite_compile_flags
        /EHsc
        /utf-8
    ) # /GF /MP

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        set(roguelite_compile_flags
            ${roguelite_compile_flags}
            /MDd
        )
    else()
        set(roguelite_compile_flags
            ${roguelite_compile_flags}
            /MD
            /O2
        )

        string(
            REGEX REPLACE
            "/RTC(su|[1su])" ""
            CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS}"
        )

        string(
            REPLACE "/RTC1" ""
            CMAKE_CXX_FLAGS_DEBUG
            ${CMAKE_CXX_FLAGS_DEBUG}
        )
    endif()

    if(CMAKE_CXX_COMPILER MATCHES "/cl$")
        # fixes C macro __ARGS__ expansion for MSVC so it's
        # consistent with other major C++ compilers (gcc/clang)
        set(roguelite_compile_flags
            ${roguelite_compile_flags}
            /Zc:preprocessor
        )
    endif()

    target_compile_options(${gdextension_lib_name} PUBLIC
        ${roguelite_compile_flags}
    )
endif()

# =======================================================================
# Dependency linkage
# =======================================================================

# link gdextension to the cpp bindings library
target_link_libraries(${gdextension_lib_name}
    PUBLIC godot::cpp
)

# =======================================================================
# Print useful configuration report
# =======================================================================

include(${CMAKE_SOURCE_DIR}/scripts/cmake/utils.cmake)
print_project_variables()
