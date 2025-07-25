cmake_minimum_required(VERSION 3.15.0)
project(
  libtrossen_arm
  VERSION 1.8.5
)

set(LIBRARY_NAME ${PROJECT_NAME})
option(BUILD_DEMOS "Build C++ Demos" OFF)
option(BUILD_DOCS "Build the documentation" OFF)

# Set the C++ standard to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the build type to Release if not already set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Determine the system architecture
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
  set(ARCH "x86_64")
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64")
  set(ARCH "aarch64")
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm64")
  set(ARCH "aarch64")
else()
  message(FATAL_ERROR "Unknown or Unsupported System Architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

# Determine the operating system
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(OS "macos")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(OS "linux")
else()
  message(FATAL_ERROR "Unknown or Unsupported Operating System: ${CMAKE_SYSTEM_NAME}")
endif()

message(STATUS "Building for OS: '${OS}', architecture: '${ARCH}'.")

add_library(${LIBRARY_NAME} STATIC IMPORTED)

target_link_libraries(${LIBRARY_NAME} INTERFACE pthread)

if(BUILD_DEMOS)
  message(STATUS "Building C++ Demos")
  add_subdirectory(demos/cpp)
endif()

set_target_properties(${LIBRARY_NAME} PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/${OS}/${ARCH}/${LIBRARY_NAME}.a"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION include
)

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib/${OS}/${ARCH}/${LIBRARY_NAME}.a
  DESTINATION lib
)

include(CMakePackageConfigHelpers)

set(PACKAGE_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(PACKAGE_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${LIBRARY_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}Config.cmake
  INSTALL_DESTINATION lib/cmake/${LIBRARY_NAME}
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMinorVersion
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}ConfigVersion.cmake
  DESTINATION
    lib/cmake/${LIBRARY_NAME}
)

include(cmake/Packaging.cmake)
include(cmake/Docs.cmake)
