cmake_minimum_required(VERSION 3.10)

project(MT19937)

find_package(Boost REQUIRED COMPONENTS multiprecision)

add_executable(MT19937 mt.cpp)

target_link_libraries(MT19937 PRIVATE Boost::multiprecision)