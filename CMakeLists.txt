cmake_minimum_required(VERSION 3.10)
project(test10)

set(CMAKE_CXX_STANDARD 11)

add_executable(test10 main.cpp HuffmanTree.h Heap.h FileCommpass.cpp FileCommpass.h)