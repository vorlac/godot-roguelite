# This is the main CMake script which (summed up) configures a CMake build project on how to build our .dll
# The gdextension .dll build can be run from Visual Studio Community, VSCode, CLion, etc as long as you have 
# installed the appropriate CMake add-ons to enable IDE/editor integration with CMake, and the project is 
# in the same directory as CMakeLists.txt

# This script can also easily handle adding 3rd party libraries to our gdextension project.
# An official explanation on how it's intended to function is here:

# https://github.com/vorlac/godot-roguelite/wiki#cmake-configuration

# =======================================================================
# Main cmake project settings area
# =======================================================================

cmake_minimum_required(VERSION 3.20)

# Name for our library:
set(GDEXTENSION_LIB_NAME roguelite)
# Directory for placing the built .dll (The project .gdextension file should likely point in here):
set(GDEXTENSION_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/project/bin")
# Tip: "set" arguments are basically defining variables in CMake

option(
  AUTOFORMAT_SRC_ON_CONFIGURE
    "If enabled, clang-format will be used to format all sources in src/ during configuration"
        ON
)
# Tip: "option" arguments are basically also CMake variables, but in a easy user toggleable format.

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

# Assign CMAKE_MODULE_PATH which will indicate future include statements where the .cmake files to be included may be located
list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-cpp/cmake/"
)
# Tip: "list" arguments allow you to group multiple variables under a single identifier.

# =======================================================================
# Configure vcpkg submodule and define library.
#   vcpkg:
#     is a submodule (repository within the main one, defined in 
#     .gitmodules) It is a program used by CMake further on in this 
#     script to easily add 3rd party libraries to our own main .dll
#     (it can priorize dynamic or static linking for the dependency 
#     as well as the C runtime based on what the VCPKG_TARGET_TRIPLET
#     variable is set to. For this project static linkage is configured
#     for the library Windows (but the C runtime is dynamically linked)
#     and Linux & MacOS favor dynamic linkage)
# =======================================================================
include(vcpkg-init)
# Tip: "vcpkg-init" is actually a .cmake script located in the cmake/ directory.
#      Include file names in the cmake/ directory don't require a full path or file extension to be specified thanks to the
#      previous CMAKE_MODULE_PATH list definition

project("${GDEXTENSION_LIB_NAME}"
  LANGUAGES
    C CXX
  VERSION
    0.1.0
)
# Tip: the "project" specifications tell CMake valuable project info for compilation: 
#      project name (same as lib name), languages and version.

include(vcpkg-install-deps)
# Tip: Includes cmake/vcpkg-install-deps.cmake and runs it here.
#      the "vcpkg-install-deps" .cmake script just makes sure that the previously asserted vcpkg install has all of it's dependencies available

# =======================================================================
# Compiler identification
# =======================================================================

set(compiler_is_clang "$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>>")
set(compiler_is_gnu "$<CXX_COMPILER_ID:GNU>")
set(compiler_is_msvc "$<CXX_COMPILER_ID:MSVC>")
# Tip: The compiler we are using is determined by configs in CMakePresets.json, which in turn are picked by the VS Community option we selected

# =======================================================================
# Configure godot-engine and godot-cpp submodules and define libraries.
#   godot-cpp:
#     Configured as a library that will statically
#     link to this project's gdextension dynamic link library.
#   godot-engine:
#     The engine submodule will be (re)built using scons if a
#     debug build of the editor doesn't already exist.
#     the engine sources and headers will then be used to declare
#     a library in cmake (but will not be built). The cmake library
#     will improve code browsing, syntax highlighting, searching,
#     debbuging, and autocomplete/intellisense for any IDEs that
#     gather data from cmake (i.e. VSCode and Visual Studio 2022)
# =======================================================================

include(godot-dev-configuration) 
# Tip: Includes cmake/godot-dev-configuration.cmake and runs it

# =======================================================================
# 3rd party library setup/configuration (leverages vcpkg)
# =======================================================================

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
# Tip: The previous packages are in vcpkg's registry, hence why CMake's find_package method can find them.
#      once found, they are respectively referenced by names (eg. fmt, spdlog, flecs)

# =======================================================================
# Our GDExtension dynamic library (.dll) setup/configuration
# =======================================================================

# Point to our library sources as "gdext_sources":
file(GLOB_RECURSE gdext_sources
     CONFIGURE_DEPENDS
       "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[hc]"
       "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[hc]pp"
)

# Use gdext_sources in order to build a shared library that will be named whatever 
# the PROJECT_NAME variable is set to (which will be the same name you set for the 
# GDEXTENSION_LIB_NAME variable at the top of this file):
add_library(${PROJECT_NAME}
    SHARED
      ${gdext_sources}
)
# Tip1: The "add_library()" command is used to specify a library target for this cmake project to build
#       The SHARED keyword tells CMake to create a shared/dynamic library. The gdext_sources file directories are passed to add_library() 
#       command to specify the source files for the library.
# Tip2: The .dll isn't built here yet, it's only being pointed at. The build will occur when running cmake --build in the same
#       directory as this CMakeLists.txt file, or if the build command is run from visual studio.

# Import compiler warnings from godot-cpp's own cmake folder:
include(common_compiler_flags)

# Set compiler options for the gdextension library based on the compiler being used:
target_compile_options(${PROJECT_NAME} PUBLIC
    $<${compiler_is_msvc}:
        /EHsc
        /utf-8
        /Zc:preprocessor
        $<$<CONFIG:Debug>:
            /MDd # System dynamic linking of C++ runtime libraries (use MT for static linking)
        >
        $<$<CONFIG:Release>:
            /MD # System dynamic linking of C++ runtime libraries (use MT for static linking)
            /O2 # Compiler will optimize code for speed
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

# Enable extension hot swapping:
target_compile_definitions(${PROJECT_NAME} PUBLIC 
	HOT_RELOAD_ENABLED
)

# Define the directories containing inclusion headers (CMake needs to know this after running add_library):
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
# Dependency linkage (linking dependencies to our .dll build)
# =======================================================================

# gdextension library dependency linkage
#     godot::cpp: Found by CMake thanks to godot-dev-configuration "add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-cpp)" statement.
#     the subdirectory addition runs godot-cpp's own CMakeLists.txt which declares "add_library(godot::cpp ALIAS ${PROJECT_NAME})"
#     CMake automatically exports all targets that are created in the godot-cpp CMakeLists.txt file. This means that the godot::cpp 
#     library is accessible and linkable from here.
#
#     The other libraries are found thanks to their definition earlier in "find_package", which 
#     is defined by the vcpkg package manager for each dependency added to the project.
target_link_libraries(${PROJECT_NAME}
    PUBLIC godot::cpp
    PRIVATE fmt::fmt
    PRIVATE fmt::fmt-header-only
    PRIVATE spdlog::spdlog_header_only
)
# Tip1: PUBLIC: our .dll depends on the library. PRIVATE: the library is expendable.
# Tip2: godot::cpp, fmt::fmt etc... each library name is declared by each of it's
#       respective projects's own CMakeLists.txt, which is reached thanks to vcpkg

# Define system architecture for the build:
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(system_bits 64)
else()
    set(system_bits 32)
endif()

# Define built .dll name:
string(TOLOWER
    "${PROJECT_NAME}.${CMAKE_SYSTEM_NAME}.${system_bits}.${CMAKE_BUILD_TYPE}"
    gde_lib_name
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES
    # This option tells CMake to generate position-independent code (PIC). 
    # PIC code can be loaded and executed at any address in memory. 
    # This is necessary when building shared/dynamic libraries.
    POSITION_INDEPENDENT_CODE ON
    # This option tells CMake to export the compile commands for the target. 
    # This can be useful for debugging and profiling purposes.	
    CMAKE_EXPORT_COMPILE_COMMANDS ON
    # This option tells CMake to enable interprocedural optimization for the target. 
    # This can improve the performance of the target by optimizing code across multiple functions.
    CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON
    # This option tells CMake to place the static library archive for the target in the specified directory.
    ARCHIVE_OUTPUT_DIRECTORY "${GDEXTENSION_LIB_PATH}"
    # This option tells CMake to place the shared library for the target in the specified directory.
    LIBRARY_OUTPUT_DIRECTORY "${GDEXTENSION_LIB_PATH}"
    # This option tells CMake to place the runtime library for the target in the specified directory.
    RUNTIME_OUTPUT_DIRECTORY "${GDEXTENSION_LIB_PATH}"
    # This option tells CMake to place the program database (PDB) file for the target in the specified directory. 
    # The PDB file contains debugging information for the target.
    CMAKE_PDB_OUTPUT_DIRECTORY "${GDEXTENSION_LIB_PATH}"
    # This option tells CMake to place the compile-time PDB file for the target in the specified directory.
    # The compile-time PDB file contains debugging information that can be used to debug the target while it is being built.
    CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${GDEXTENSION_LIB_PATH}"
    # This option tells CMake to set the output name for the target to the specified value.
    OUTPUT_NAME "${gde_lib_name}"
)

# =======================================================================
# Print configuration report
# =======================================================================

# Include utility script that prints a handful of useful build/configuration cmake variables:
include(cmake-utils)
print_project_variables()
