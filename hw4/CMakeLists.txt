project(hw4)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

add_library( HuffmanLib STATIC
             compressor.cpp bitvector.cpp )

add_executable(hw4 main.cpp)


TARGET_LINK_LIBRARIES( hw4 HuffmanLib )

set_property(TARGET hw4 PROPERTY CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined  -Werror -D_GLIBCXX_DEBUG")
endif()
