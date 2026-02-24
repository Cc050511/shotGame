cmake_minimum_required(VERSION 3.30)

project(
  shotGame
  DESCRIPTION "a simple shooting game")

set(CMAKE_CXX_STANDARD 23)

find_package(SDL3 REQUIRED CONFIG)

file(GLOB SOURCES "src/*.cpp")

add_executable(shotGame ${SOURCES})

# 链接 SDL3 目标
target_link_libraries(shotGame PRIVATE SDL3::SDL3)