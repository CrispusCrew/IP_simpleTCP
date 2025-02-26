if(BUILD_SERVER)
    add_executable(server main.cpp server.cpp logger.cpp)
else()
    add_library(server STATIC server.cpp)
endif()
set(SERVER_SOURCES )

target_include_directories(server PUBLIC ${SRC_DIR} ${LIB_DIR} ${ASIO_INCLUDE_DIR})