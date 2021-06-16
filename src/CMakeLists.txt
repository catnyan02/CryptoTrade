cmake_minimum_required(VERSION 3.10.0)
project(CryptoTrade)
include(${CMAKE_CURRENT_LIST_DIR}/../cmake/Tester.cmake)
auto_tester(src main.cpp test.cpp)

#for gtkmm
#link_directories(
#        ${GTKMM_LIBRARY_DIRS}  )
#
#include_directories(
#        ${GTKMM_INCLUDE_DIRS}  )

#add_executable(example # name of the executable on Windows will be example.exe
#        main.cc
#        radiobuttons.cc
#        radiobuttons.h )

#target_link_libraries(example
#        ${GTKMM_LIBRARIES}  )