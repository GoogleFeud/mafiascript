
add_definitions("/std:c++17 /Qspectre- -DNNG_STATIC_LIB")

add_executable (Mafiascript 
"main.cpp"
)

target_include_directories(Mafiascript PUBLIC include)

install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_DIR})