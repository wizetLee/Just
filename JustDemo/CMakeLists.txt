cmake_minimum_required(VERSION 3.25)
project(JustDemo)

set(CMAKE_CXX_STANDARD 11)

add_executable(JustDemo main.cpp src/zmq/zmq_util.cpp src/zmq/zmq_util.h)

#set(ZEROMQ_VERSION 4.3.4)
#set(LIBEVENT_VERSION 2.1.12)

set(ZEROMQ_VERSION 4.3.5_1)
set(LIBEVENT_VERSION 2.1.12_1)

include_directories(/opt/homebrew/Cellar/zeromq/${ZEROMQ_VERSION}/include)
target_link_libraries(JustDemo PRIVATE /opt/homebrew/Cellar/zeromq/${ZEROMQ_VERSION}/lib/libzmq.dylib)

include_directories(/opt/homebrew/Cellar/libevent/${LIBEVENT_VERSION}/include)
target_link_libraries(JustDemo PRIVATE /opt/homebrew/Cellar/libevent/${LIBEVENT_VERSION}/lib/libevent.dylib)