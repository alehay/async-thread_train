
cmake_minimum_required(VERSION 3.5)

project(Boost_asio_test)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-ggdb )

add_executable(lazyGenerator lazyGenerator.cpp)
add_executable(infiniteDataStream infiniteDataStream.cpp)