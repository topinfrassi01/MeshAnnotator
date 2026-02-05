cmake_minimum_required(VERSION 3.19)

# Detect and configure vcpkg toolchain automatically when CMAKE_TOOLCHAIN_FILE is not provided.
# Priority: $VCPKG_ROOT environment variable -> project-local "vcpkg" folder.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(_vcpkg_toolchain "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
  if(EXISTS "${_vcpkg_toolchain}")
    message(STATUS "Found vcpkg toolchain at ${_vcpkg_toolchain}; setting CMAKE_TOOLCHAIN_FILE")
    set(CMAKE_TOOLCHAIN_FILE "${_vcpkg_toolchain}" CACHE FILEPATH "Vcpkg toolchain file")
  else()
    message(STATUS "vcpkg toolchain not found (checked ${_vcpkg_toolchain}). If you use vcpkg, set VCPKG_ROOT or pass -DCMAKE_TOOLCHAIN_FILE=<path>.")
  endif()
endif()

project(MeshAnnotator LANGUAGES CXX)

# Use C++11 (no compiler extensions)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Prevent in-source builds
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed. Please create a separate build directory (e.g. 'build').")
endif()

option(BUILD_TESTING "Enable building unit tests" ON)

# Project layout
add_subdirectory(src/VTKLayer)
add_subdirectory(src/MeshAnnotator)

message(STATUS "Project configured: ${PROJECT_NAME} (C++${CMAKE_CXX_STANDARD})")