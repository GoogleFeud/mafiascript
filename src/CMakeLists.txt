add_executable(${PROJECT_NAME} main.cpp)

set (CMAKE_CXX_FLAGS "/std:c++17 /Qspectre-")

install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_DIR})