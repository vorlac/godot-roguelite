# =======================================================================
# Godot Engine submodule update/init
# =======================================================================

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
# Godot-cpp bindings submodule update/init
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

string(TOLOWER "${CMAKE_SYSTEM_NAME}" host_os)
set(cpu_arch "x86_64")

# define variable to be used in the engine build when specifying platform.
set(host_os_engine "${host_os}")
if (APPLE)
    if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm64")
            set(cpu_arch "arm64")
    endif()
    # ${CMAKE_SYSTEM_NAME} returns Darwin, but the scons platform name will be macos
    set(host_os_engine "macos")
elseif(UNIX)
    # the scons build expects linuxbsd to be passed in as the platform
    # when building on linux, so just append bsd to CMAKE_SYSTEM_NAME
    set(host_os_engine "${host_os}bsd")
endif()


set(godot_debug_editor_executable
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/bin/godot.${host_os_engine}.editor.dev.${cpu_arch}${CMAKE_EXECUTABLE_SUFFIX}"
)

find_program(SCONS_PROGRAM NAMES scons)
if (NOT EXISTS "${SCONS_PROGRAM}")
    message(FATAL_ERROR
      "scons not found, it is required for the godot engine build. "
      "Please install scons and confirm it is in your system PATH."
    )
endif()

message(NOTICE "godot_debug_editor_executable = ${godot_debug_editor_executable}")

# if the engine/editor executable isn't found in the
# engine's submodule bin folder, invoke the scons build.
if(NOT EXISTS "${godot_debug_editor_executable}")
    message(STATUS "Godot engine debug binaries not found, invoking debug build of engine...")

    if (WIN32)
        set(SCONS_COMMAND powershell -c)
    endif()
    
    set(SCONS_COMMAND 
      ${SCONS_COMMAND}
      ${SCONS_PROGRAM} 
          target=editor 
          use_static_cpp=yes 
          dev_build=yes 
          debug_symbols=yes 
          optimize=none 
          use_lto=no
    )

    set(GODOT_ENGINE_CLEAN_BUILD OFF)
    if (GODOT_ENGINE_CLEAN_BUILD MATCHES ON)
        message(STATUS "Invoking scons clean: ${SCONS_COMMAND} --clean")
        
        execute_process(
            COMMAND "${SCONS_PROGRAM}" --clean
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine"
            COMMAND_ERROR_IS_FATAL ANY
        )
    endif()

    message(STATUS "Invoking scons build: ${SCONS_COMMAND}")
    # this build should only ever need to be run once (unless the enging debug binaries
    # are deleted or you want to change the build configuration/command invoked below).
    execute_process(
        COMMAND ${SCONS_COMMAND}
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

# add the engine sources as a library so intellisense works in VS and VSCode
# (and any other IDEs that support CMake in a way where the information from
# the CMake build is fed into the IDE for additional context about the code
# when browsing/debugging). even though the engine is being added as a library here,
# the EXCLUDE_FROM_ALL option will prevent it from compiling. This is done
# purely for IDE integration so it's able to properly navigate the engine
# source code using features like "go do definition", or typical tooltips.
add_library(godot_engine EXCLUDE_FROM_ALL ${godot_engine_sources})

# this is just a handful of additional include directories used by the engine.
# this isn't a complete list, I just add them as needed whenever I venture into
# code where the IDE can't find certain header files during engine source browsing.
target_include_directories(godot_engine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/platform/windows"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/thirdparty/zlib"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/thirdparty/vulkan"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/thirdparty/vulkan/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/thirdparty/vulkan/include/vulkan"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/drivers/vulkan"
    SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/thirdparty/glad"
    SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/thirdparty/volk"
    SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/thirdparty/zstd"
    SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-engine/thirdparty/mbedtls/include"
)

# define a bunch of the same symbol definitions
# used when by the scons engine build. These build
# flags can differen based on the engine's build for
# you system. Update as needed for your setup.
target_compile_definitions(godot_engine PUBLIC
    $<$<CONFIG:Debug>:
        DEBUG_ENABLED
        DEBUG_METHODS_ENABLED
        DEV_ENABLED
    >
    $<$<BOOL:UNIX>:
        UNIX_ENABLED
        VK_USE_PLATFORM_XLIB_KHR
    >
    $<$<BOOL:WIN32>:
        WINDOWS_ENABLED
        WASAPI_ENABLED
        WINMIDI_ENABLED
        TYPED_METHOD_BIND
        NOMINMAX
        WIN32
        VK_USE_PLATFORM_WIN32_KHR
        _SCRT_STARTUP_WINMAIN=1
        $<$<BOOL:MSVC>:
            MSVC
        >
    >
    TOOLS_ENABLED
    NO_EDITOR_SPLASH
    GLAD_ENABLED
    GLES3_ENABLED
    GLES_OVER_GL
    VULKAN_ENABLED
    USE_VOLK
    MINIZIP_ENABLED
    BROTLI_ENABLED
    ZSTD_STATIC_LINKING_ONLY
)
