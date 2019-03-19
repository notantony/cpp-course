project(hw6)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

INCLUDE_DIRECTORIES( ${MY_SOURCE_DIR})
add_executable(hw6
                    gtest/gtest_main.cc
                    gtest/gtest-all.cc
                    gtest/gtest.h
                    bind.h
                    test.cpp
                    )

#add_executable(hw6
#                    main.cpp
#                    bind.h
#                    )


set_property(TARGET hw6 PROPERTY CXX_STANDARD 14)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined  -Werror -D_GLIBCXX_DEBUG")
endif()

target_link_libraries(hw6 -lpthread)
