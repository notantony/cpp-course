cmake_minimum_required(VERSION 2.8)

project(BIGINT)

include_directories(${BIGINT_SOURCE_DIR})

add_executable(BIGINT big_integer_testing.cpp big_integer.h big_integer.cpp vector.h vector.cpp gtest/gtest-all.cc gtest/gtest.h gtest/gtest_main.cc)
#add_executable(BIGINT test.cpp big_integer.h big_integer.cpp vector.h vector.cpp)
set_property(TARGET BIGINT PROPERTY CXX_STANDARD 11)


if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG -Wall")
endif()


target_link_libraries(BIGINT -lgmp -lgmpxx -lpthread)
