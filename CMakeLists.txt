cmake_minimum_required(VERSION 3.20)
project(bithacks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(bithacks INTERFACE)
target_include_directories(bithacks INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

enable_testing()
add_subdirectory(tests)

add_executable(demo_all examples/demo_all.cpp)
target_link_libraries(demo_all PRIVATE bithacks)
