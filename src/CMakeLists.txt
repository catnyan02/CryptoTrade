cmake_minimum_required(VERSION 3.10.0)
project(CryptoTrade)
include(${CMAKE_CURRENT_LIST_DIR}/../cmake/Tester.cmake)
auto_tester(src main.cpp test.cpp)