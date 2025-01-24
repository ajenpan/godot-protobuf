cmake_minimum_required(VERSION 3.13)
project(godot-protobuf)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

set(GODOT_GDEXTENSION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/godot-cpp/gdextension CACHE STRING "Path to GDExtension interface header directory")
set(CPP_BINDINGS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/godot-cpp CACHE STRING "Path to C++ bindings")

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(GODOT_CPP_BUILD_TYPE Debug)
else()
	set(GODOT_CPP_BUILD_TYPE Release)
endif()

string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME)
string(TOLOWER ${GODOT_CPP_BUILD_TYPE} BUILD_TYPE)
string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} ARCHITECTURE)

message("CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE} GODOT_CPP_BUILD_TYPE:${GODOT_CPP_BUILD_TYPE}")

if(ANDROID)
	# Added the android abi after system name
	set(SYSTEM_NAME ${SYSTEM_NAME}.${ANDROID_ABI})
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(TARGET_PATH x11)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(TARGET_PATH win64)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(TARGET_PATH macos)
else()
	message(FATAL_ERROR "Not implemented support for ${CMAKE_SYSTEM_NAME}")
endif()

# Change the output directory to the bin directory
set(BUILD_PATH ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_PATH}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BUILD_PATH}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BUILD_PATH}")

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BUILD_PATH}")
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BUILD_PATH}")
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${BUILD_PATH}")
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${BUILD_PATH}")
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${BUILD_PATH}")
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${BUILD_PATH}")

# Set the c++ standard to c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# using Visual Studio C++
	set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} /DTYPED_METHOD_BIND")

	# Disable conversion warning, truncation, unreferenced var, signed mismatch
	set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} /wd4244 /wd4305 /wd4101 /wd4018 /wd4267")

	add_definitions(-DNOMINMAX)
else()
	set(GODOT_LINKER_FLAGS "-static-libgcc -static-libstdc++ -Wl,-R,'$$ORIGIN'")

	set(GODOT_COMPILE_FLAGS "-fPIC -g -Wwrite-strings")

	if(CMAKE_BUILD_TYPE MATCHES Debug)
		set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -fno-omit-frame-pointer -O0")
	else()
		set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -O3")
	endif(CMAKE_BUILD_TYPE MATCHES Debug)
endif()

# Disable exception handling. Godot doesn't use exceptions anywhere, and this
# saves around 20% of binary size and very significant build time (GH-80513).
option(GODOT_DISABLE_EXCEPTIONS ON "Force disabling exception handling code")

if(GODOT_DISABLE_EXCEPTIONS)
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -D_HAS_EXCEPTIONS=0")
	else()
		set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -fno-exceptions")
	endif()
else()
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} /EHsc")
	endif()
endif()

# Get Sources
file(GLOB_RECURSE SOURCES src/*.c**)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "${PROJECT_NAME}_${SYSTEM_NAME}_${BUILD_TYPE}_${ARCHITECTURE}"
	LIBRARY_OUTPUT_DIRECTORY ${BUILD_PATH}
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
	${CPP_BINDINGS_PATH}/include
	${CPP_BINDINGS_PATH}/gen/include
	${GODOT_GDEXTENSION_DIR}
)

# protobuf
# set(protobuf_BUILD_PROTOC_BINARIES OFF)
set(protobuf_BUILD_TESTS OFF)
set(protobuf_BUILD_LIBUPB OFF)
set(protobuf_WITH_ZLIB_DEFAULT OFF)
set(protobuf_WITH_ZLIB OFF)
add_subdirectory(protobuf)
target_link_libraries(${PROJECT_NAME} PRIVATE protobuf::libprotobuf)

# godot-cpp
add_subdirectory(godot-cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE godot::cpp)

# Add the compile flags
set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS ${GODOT_COMPILE_FLAGS})
set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS ${GODOT_LINKER_FLAGS})
