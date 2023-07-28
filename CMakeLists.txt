cmake_minimum_required(VERSION 3.19)
project(roguelite LANGUAGES C CXX)

set(gdextension_lib_name ${PROJECT_NAME})

# turn this on to print all CMAKE cache variables
# and output a dependecy tree of all dependencies
# option(ASAN_ENABLED "Build this target with AddressSanitizer" ON)
option(ROGUELITE_PRINT_DEPENDENCY_TREE OFF "If enabled cmake will output a dependency tree after configuration")
option(ROGUELITE_PRINT_CMAKE_VARIABLES OFF "if enabled all cmake cache variables will be printed after configuration")

# enable by default for debug build configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(PRINT_CMAKE_DIAGNOSTICS ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(${CMAKE_SOURCE_DIR}/scripts/cmake/utils.cmake)
include(FetchContent)

set(CMAKE_DEBUG_POSTFIX "d")

FetchContent_Declare(
	GDExtension
	GIT_REPOSITORY https://github.com/godotengine/godot-cpp.git
	GIT_TAG godot-4.1-stable
)

FetchContent_MakeAvailable(GDExtension)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${gdextension_lib_name}/bin)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/bin)

# Automatically pick up files added to src
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

set(ROGUELITE_COMPILE_FLAGS)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# using Visual Studio C++
	set(ROGUELITE_COMPILE_FLAGS "/EHsc") # /GF /MP

	if(CMAKE_BUILD_TYPE MATCHES Debug)
		set(ROGUELITE_COMPILE_FLAGS "${ROGUELITE_COMPILE_FLAGS} /MDd") # /Od /RTC1 /Zi
	else()
		set(ROGUELITE_COMPILE_FLAGS "${ROGUELITE_COMPILE_FLAGS} /MD /O2") # /Oy /GL /Gy
		STRING(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
		string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
	endif(CMAKE_BUILD_TYPE MATCHES Debug)

	add_definitions(-DNOMINMAX)
else() # GCC/Clang
	set(ROGUELITE_COMPILE_FLAGS "${ROGUELITE_COMPILE_FLAGS} -g")

	if(CMAKE_BUILD_TYPE MATCHES Debug)
		set(ROGUELITE_COMPILE_FLAGS "${ROGUELITE_COMPILE_FLAGS} -fno-omit-frame-pointer -O0")
	else()
		set(ROGUELITE_COMPILE_FLAGS "${ROGUELITE_COMPILE_FLAGS} -O3")
	endif(CMAKE_BUILD_TYPE MATCHES Debug)
endif()

if(MSVC)
	# fixes C macro __ARGS__ expansion for MSVC so it's
	# consistent with other major C++ compilers (gcc/clang)
	if(CMAKE_CXX_COMPILER MATCHES "/cl$")
		target_compile_options(${PROJECT_NAME} PRIVATE
			/Zc:preprocessor
			${ROGUELITE_COMPILE_FLAGS}
		)
	elseif(CMAKE_CXX_COMPILER MATCHES "/clang-cl$")
		# adjusts the number of errors output by clang
		set(extra_compile_defs ${extra_compile_defs}
			-ferror-limit=100
		)
	endif()

	# force disable the debug heap for any
	# non-debug project build configurations
	if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(extra_compile_defs ${extra_compile_defs}
			_NO_DEBUG_HEAP=1
		)
	endif()
endif()

# Add the gdextension dynamic library
add_library(${gdextension_lib_name} SHARED ${SOURCES})

# set compiler definitions that match up with the buid used for the editor:
# scons platform=windows arch=x86_64 target=editor dev_build=yes
# debug_symbols=yes verbose=yes warnings=extra werror=yes
target_compile_definitions(${PROJECT_NAME} PRIVATE
	${extra_compile_defs}
	NOMINMAX
	TOOLS_ENABLED
	DEBUG_ENABLED
	DEV_ENABLED
	NO_EDITOR_SPLASH
	WINDOWS_ENABLED
	WASAPI_ENABLED
	WINMIDI_ENABLED
	TYPED_METHOD_BIND
	WIN32
	MSVC
	WINVER=0x0601
	_WIN32_WINNT=0x0601
	NOMINMAX
	_WIN64
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

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME} PUBLIC godot::cpp)

# source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX src FILES ${SOURCES})

# =======================================
# CMAKE configuration diagnostics
# =======================================
# prints dependency tree if the option is set
if(ROGUELITE_PRINT_DEPENDENCY_TREE MATCHES ON)
	set_property(GLOBAL PROPERTY GLOBAL_DEPENDS_DEBUG_MODE ON)
endif()

# output cmake variables if option is set
if(ROGUELITE_PRINT_CMAKE_VARIABLES MATCHES ON)
	dump_cmake_variables(".*")
endif()

# print cmake project configuration variables
print_project_variables()
