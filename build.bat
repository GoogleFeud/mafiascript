	cd ./dist
    cmake .. || exit 1
    cmake --build . --target INSTALL --config Release