# VCPKG submodule init/update
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/vcpkg/ports")
    message(NOTICE "VCPKG package manager sources not found")
    message(NOTICE "initializing/updating the vcpkg submodule...")
    execute_process(
        COMMAND git submodule update --init extern/vcpkg
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

# =======================================================================
# VCPKG triplet definition (should enforce static linkage for all deps)
# This would typically be passed in from CMakePresets.json, but if
# the cmake configuration is invoked without using a preset this fallback
# should define a preset that prefers static linkage for 3rd party libs.
# =======================================================================

if (NOT VCPKG_TARGET_TRIPLET)
    if (WIN32)
        # static-md enforces static linkage to all dependencies,
        # as well as dynamic linkage to the C runtime for consistency.
        # if this gives you trouble change to "x64-windows-static".
        set(VCPKG_TARGET_TRIPLET "x64-windows-static-md")
    elseif(APPLE)
        if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm64")
            set(VCPKG_TARGET_TRIPLET "arm64-osx")
        else()
            set(VCPKG_TARGET_TRIPLET "x64-osx")
        endif()
    elseif(UNIX)
        set(VCPKG_TARGET_TRIPLET "x64-linux")
    endif()
endif()

# =======================================================================
# Define VCPKG toolchain file. This would typically be passed in from
# CMakePresets.json, but if the cmake configuration is invoked without
# using a preset this fallback should detect that the path is missing.
# =======================================================================
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(toolchain_file_path "${CMAKE_CURRENT_SOURCE_DIR}/extern/vcpkg/scripts/buildsystems/vcpkg.cmake")
    if (EXISTS "${toolchain_file_path}")
        set(CMAKE_TOOLCHAIN_FILE "${toolchain_file_path}")
    else()
        message(WARNING "VCPKG toolchain file not found: ${toolchain_file_path}")
    endif()
endif()
