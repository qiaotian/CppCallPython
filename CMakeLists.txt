cmake_minimum_required(VERSION 2.8)
project(python_test)
set(CMAKE_BUILD_TYPE Debug)

find_package(Boost COMPONENTS thread serialization python REQUIRED)
include_directories(${Boost_INCLUDE_DIR} 
/usr/include/python2.7)

add_executable(seg seg.cpp)

target_link_libraries(seg
    ${Boost_LIBRARIES}
    /usr/lib/x86_64-linux-gnu/libboost_python-py27.so
    /usr/lib/x86_64-linux-gnu/libstdc++.so.6
    /usr/lib/x86_64-linux-gnu/libpython2.7.so
    ) 
