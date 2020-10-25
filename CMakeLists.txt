cmake_minimum_required(VERSION 3.10)

project(Mafiascript)

set(INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/${CMAKE_BUILD_TYPE}")

add_subdirectory(src)