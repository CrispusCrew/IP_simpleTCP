# cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -B build
# cmake --build build --target client server
# cmake --install build

set(CMAKE_BUILD_TYPE Release)

project(simpleTCP)

cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_CLIENT "Build client executable" ON)
option(BUILD_SERVER "Build server executable" ON)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(SRC_FILES ${SRC_DIR}/utilities.cpp)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(ASIO_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib/asio)
set(LOG_DIR ${PROJECT_SOURCE_DIR}/log)

file(MAKE_DIRECTORY ${LOG_DIR})

add_subdirectory(client)
add_subdirectory(server)

target_sources(server PRIVATE ${SRC_FILES})
target_sources(client PRIVATE ${SRC_FILES})

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
install(TARGETS client server RUNTIME DESTINATION bin)