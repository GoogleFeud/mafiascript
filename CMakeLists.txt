cmake_minimum_required(VERSION 3.10)

# set the project name
project(Mafiascript)

# add the executable
set(INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/${CMAKE_BUILD_TYPE}")

add_subdirectory(src)