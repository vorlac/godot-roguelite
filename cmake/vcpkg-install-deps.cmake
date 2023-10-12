# =======================================================================
# VCPKG bootstrap / initialization.
# =======================================================================

set(vcpkg_executable "${CMAKE_CURRENT_SOURCE_DIR}/extern/vcpkg/vcpkg${CMAKE_EXECUTABLE_SUFFIX}")

if(EXISTS "${vcpkg_executable}")
    message(NOTICE "Found VCPKG Executable: ${vcpkg_executable}")
else()
    message(NOTICE "Could not find VCPKG Executable: ${vcpkg_executable}")
    message(NOTICE "Calling VCPKG bootstrap scripts.")

    if(WIN32)
        execute_process(
            COMMAND powershell -c "${CMAKE_CURRENT_SOURCE_DIR}/extern/vcpkg/bootstrap-vcpkg.bat"
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
