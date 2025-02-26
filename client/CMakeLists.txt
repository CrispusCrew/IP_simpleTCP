if(BUILD_CLIENT)
    add_executable(client main.cpp client.cpp)
else()
    add_library(client STATIC client.cpp)
endif()

target_include_directories(client PUBLIC ${SRC_DIR} ${LIB_DIR} ${ASIO_INCLUDE_DIR})