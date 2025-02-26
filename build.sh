cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -B build
cmake --build build --target client server
cmake --install build
chmod a+x bin/*