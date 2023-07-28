cmake_minimum_required(VERSION 3.19)
project(roguelite LANGUAGES C CXX)

set(gdextension_lib_name ${PROJECT_NAME})

include(${CMAKE_SOURCE_DIR}/scripts/cmake/utils.cmake)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_DEBUG_POSTFIX "d")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/bin)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/bin)

FetchContent_Declare(
	GDExtension
	GIT_REPOSITORY https://github.com/godotengine/godot-cpp.git
	GIT_TAG godot-4.1-stable
)

FetchContent_MakeAvailable(GDExtension)

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

# do the same for the engine source files 
file(GLOB_RECURSE engine_sources CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/engine/*.[hc]"
	"${CMAKE_CURRENT_SOURCE_DIR}/engine/*.[hc]pp"
)

# add the engine sources as a library so intellisense actually works
add_library(engine EXCLUDE_FROM_ALL ${engine_sources})
target_include_directories(engine PUBLIC 
	"${CMAKE_CURRENT_SOURCE_DIR}/engine"
)


if(MSVC)
	# using Visual Studio C++
	set(roguelite_compile_flags "/EHsc") # /GF /MP

	if(CMAKE_BUILD_TYPE MATCHES Debug)
		set(roguelite_compile_flags "${roguelite_compile_flags} /MDd") # /Od /RTC1 /Zi
	else()
		set(roguelite_compile_flags "${roguelite_compile_flags} /MD /O2") # /Oy /GL /Gy
		STRING(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
		string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
	endif(CMAKE_BUILD_TYPE MATCHES Debug)

	# fixes C macro __ARGS__ expansion for MSVC so it's
	# consistent with other major C++ compilers (gcc/clang)
	if(CMAKE_CXX_COMPILER MATCHES "/cl$")
		target_compile_options(${PROJECT_NAME} PRIVATE
			/Zc:preprocessor
			${roguelite_compile_flags}
		)
	endif()
endif()

# set compiler definitions that match up with the buid used for the editor:
# scons platform=windows arch=x86_64 target=editor dev_build=yes
# debug_symbols=yes verbose=yes warnings=extra werror=yes
target_compile_definitions(${PROJECT_NAME} PUBLIC 
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

# link gdextension to the cpp bindings library
target_link_libraries(${PROJECT_NAME} PUBLIC godot::cpp)

# print cmake configuration variables
print_project_variables()
