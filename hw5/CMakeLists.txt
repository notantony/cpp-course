project(hw5)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

INCLUDE_DIRECTORIES( ${MY_SOURCE_DIR})
add_executable(hw5 main.cpp persistent_set.h shared_ptr.h linked_ptr.h tests.cpp)


set_property(TARGET hw5 PROPERTY CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined  -Werror -D_GLIBCXX_DEBUG")
endif()
