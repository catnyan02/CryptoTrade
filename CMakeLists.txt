cmake_minimum_required(VERSION 3.10.0)
project(CryptoTrade)
enable_testing()
add_subdirectory(external/doctest)
include(external/doctest/doctest.cmake)
add_subdirectory(src)
