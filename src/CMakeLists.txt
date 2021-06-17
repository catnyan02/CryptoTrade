cmake_minimum_required(VERSION 3.10.0)
project(CryptoTrade)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)
 
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  # Specify a GIT_TAG here.
)
 
FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
 
 
 

 
# C++17 is used. We requires fold expressions at least.
add_executable(proj_test test.cpp lib.cpp)
add_executable(proj main.cpp lib.cpp)
target_link_libraries(proj_test PUBLIC doctest::doctest)
target_link_libraries(proj
  PUBLIC ftxui::component
)
target_link_libraries(proj PUBLIC doctest::doctest)
doctest_discover_tests(proj_test)
set_property(TARGET proj PROPERTY CXX_STANDARD 17)
set_property(TARGET proj_test PROPERTY CXX_STANDARD 17)
# include(${CMAKE_CURRENT_LIST_DIR}/../cmake/Tester.cmake)
# auto_tester(src main.cpp test.cpp)

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