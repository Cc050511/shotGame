cmake_minimum_required(VERSION 3.30)

project(
  shotGame
  DESCRIPTION "a simple shooting game")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED CONFIG)

file(GLOB SOURCES "src/*.cpp")

add_executable(shotGame ${SOURCES})

target_link_libraries(shotGame PRIVATE SDL3::SDL3)

# Copy assets to build directory
add_custom_command(TARGET shotGame POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:shotGame>/assets")