cmake_minimum_required(VERSION 3.25)
project(JustClientDemo)

set(CMAKE_CXX_STANDARD 11)

add_executable(JustClientDemo main.cpp src/zmq/zmq_util.cpp src/zmq/zmq_util.h)

include_directories(/opt/homebrew/Cellar/zeromq/4.3.5_1/include)
target_link_libraries(JustClientDemo PRIVATE /opt/homebrew/Cellar/zeromq/4.3.5_1/lib/libzmq.dylib)
