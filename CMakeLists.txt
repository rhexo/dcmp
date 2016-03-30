# Specify the minimum version of Cmake
cmake_minimum_required(VERSION 2.8)

# Project`s name
project(mpdc)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Compile with C++11 support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#include boost support
#find_package( Boost 1.55 COMPONENTS program_options REQUIRED )

set(local_include "/usr/local/include")

# The folowing folder will be included
#include_directories( ${Boost_INCLUDE_DIR}  "${PROJECT_SOURCE_DIR}/include" ${local_include})
include_directories( "${PROJECT_SOURCE_DIR}/include" ${local_include})

#file(GLOB SOURCES "*.cpp")

# Create application executable
add_executable(mpdc
  main.cpp
  include/mpdc.hpp
  mpdc.cpp)


# Support for libpqxx
#set(pqxx_lib "/usr/local/lib/libpqxx.so")

# find_library(DBACC_LIBRARY NAMES dbacc PATHS ${CMAKE_BINARY_DIR} NO_DEFAULT_PATH)
# link with boost libraries
# target_link_libraries(websnap ${Boost_LIBRARIES} ${DBACC_LIBRARY})
