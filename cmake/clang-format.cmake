# SPDX-License-Identifier: Unlicense

find_program(CLANG_FORMAT_PROGRAM NAMES clang-format)

if (CLANG_FORMAT_PROGRAM)
    execute_process(
        COMMAND "${CLANG_FORMAT_PROGRAM}" --version
        OUTPUT_VARIABLE CLANG_FORMAT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    message("Using clang-format: ${CLANG_FORMAT_PROGRAM} (${CLANG_FORMAT_VERSION})")

    file(GLOB_RECURSE 
      format_src_list 
      RELATIVE 
        "${CMAKE_CURRENT_SOURCE_DIR}"
          "src/*.[hc]"
          "src/*.[hc]pp"
    )

    foreach(_src_file ${format_src_list})
        message("    formatting => ${_src_file}")
        execute_process( 
          COMMAND "${CLANG_FORMAT_PROGRAM}" --style=file -i "${_src_file}"
          WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        )
    endforeach()

    unset(CLANG_FORMAT_VERSION)
endif()
