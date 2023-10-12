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
