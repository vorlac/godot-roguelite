cmake_minimum_required(VERSION 3.20)

set(GDEXTENSION_LIB_NAME roguelite)
set(GDEXTENSION_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/project/bin")

option(
  AUTOFORMAT_SRC_ON_CONFIGURE
    "If enabled, clang-format will be used to format all sources in src/ during configuration"
        ON
)

option(
  USE_CCACHE_FOR_GDEXT_BUILD
    "If enabled, ccache will be used to when building the project lib"
      ON
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_MESSAGE_LOG_LEVEL STATUS)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-cpp/cmake/"
)

include(vcpkg-init)

project("${GDEXTENSION_LIB_NAME}"
  LANGUAGES
    C CXX
  VERSION
    0.1.0
)

include(vcpkg-install-deps)

# =======================================================================
# Compiler identification
# =======================================================================

set(compiler_is_clang "$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>>")
set(compiler_is_gnu "$<CXX_COMPILER_ID:GNU>")
set(compiler_is_msvc "$<CXX_COMPILER_ID:MSVC>")

# =======================================================================
# Configure godot-engine and godot-cpp submodules and define libraries.
#   godot-cpp:
#     configured as a library that will statically
#     link this project's gdextension library.
#   godot-engine:
#     the engine submodule will be (re)built using scons if a
#     debug build of the editor doesn't already exist.
#     the engine sources and headers will then be used to declare
#     a library in cmake (but will not built). the cmake library
#     will improve code browsing, syntax highlighting, searching,
#     debbuging, and autocomplete/intellisense for any IDEs that
#     gather data from cmake (i.e. VSCode and Visual Studio 2022)
# =======================================================================

include(godot-dev-configuration)

# =======================================================================
# 3rd party library setup/configuration (leverages vcpkg)
# =======================================================================

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# =======================================================================
# GDExtension dynamic library setup/configuration
# =======================================================================

# add library sources
file(GLOB_RECURSE gdext_sources
     CONFIGURE_DEPENDS
       "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[hc]"
       "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[hc]pp"
)

# add the gdextension dynamic library
add_library(${PROJECT_NAME}
    SHARED
      ${gdext_sources}
)

# import compiler warningS from godot-cpp
include(GodotCompilerWarnings)

# set compiler options for the gdextension library based on the compiler being used
target_compile_options(${PROJECT_NAME} PUBLIC
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
            -Wno-attributes
            -Wno-attributes=rl::
        >
        $<${compiler_is_clang}:
            -Wno-ignored-attributes
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

# include directories gdextension library
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if (NOT APPLE)
    # linker options for the gdextension library
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<NOT:${compiler_is_msvc}>:
            -static-libgcc
            -static-libstdc++
            -Wl,-R,'$$ORIGIN'
        >
    )
endif()

# =======================================================================
# Optional configuration / build features
# =======================================================================

if (USE_CCACHE_FOR_GDEXT_BUILD MATCHES ON)
    include(ccache)
endif()

if (AUTOFORMAT_SRC_ON_CONFIGURE MATCHES ON)
    include(clang-format)
endif()

# =======================================================================
# Dependency linkage
# =======================================================================

# gdextension library dependency linkage
target_link_libraries(${PROJECT_NAME}
    PUBLIC godot::cpp
    PRIVATE fmt::fmt
    PRIVATE fmt::fmt-header-only
    PRIVATE spdlog::spdlog_header_only
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(system_bits 64)
else()
    set(system_bits 32)
endif()

string(TOLOWER
    "${PROJECT_NAME}.${CMAKE_SYSTEM_NAME}.${system_bits}.${CMAKE_BUILD_TYPE}"
    gde_lib_name
)

set_target_properties(${PROJECT_NAME}
	PROPERTIES
		POSITION_INDEPENDENT_CODE ON
        CMAKE_EXPORT_COMPILE_COMMANDS ON
        CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON
		ARCHIVE_OUTPUT_DIRECTORY "${GDEXTENSION_LIB_PATH}"
		LIBRARY_OUTPUT_DIRECTORY "${GDEXTENSION_LIB_PATH}"
		RUNTIME_OUTPUT_DIRECTORY "${GDEXTENSION_LIB_PATH}"
        CMAKE_PDB_OUTPUT_DIRECTORY "${GDEXTENSION_LIB_PATH}"
        CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${GDEXTENSION_LIB_PATH}"
		OUTPUT_NAME "${gde_lib_name}"
)

# =======================================================================
# Print configuration report
# =======================================================================

# include utility script that prints a handful of
# useful build/configuration cmake variables
include(cmake-utils)
print_project_variables()
