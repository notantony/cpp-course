project(list)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

include_directories(${BIGINT_SOURCE_DIR})

add_executable( list
                list.h
                #main.cpp
                tests.cpp
                gtest/gtest-all.cc
                gtest/gtest.h
                gtest/gtest_main.cc
                )

set_property(TARGET list PROPERTY CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined  -Werror -D_GLIBCXX_DEBUG")
endif()

target_link_libraries( list -lpthread)
