cmake_minimum_required(VERSION 3.16)
project(chess)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++2a")
set(CMAKE_BUILD_TYPE Debug)

set(OpenGlLinkers -lglfw3 -lpthread -lm -lz -lGL -lX11 -lXext -lXfixes -ldl -lGLEW)

add_executable(chess main.cpp
        board.cpp)
add_executable(code_generator networking/gen_code.cxx
        networking/handler.cpp)
add_executable(chess_cli chess_cli.cxx
        player.cpp
        board.cpp
        networking/master.cpp
        networking/slave.cpp
        networking/handler.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenGlLinkers})