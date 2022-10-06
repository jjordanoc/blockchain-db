cmake_minimum_required(VERSION 3.22)

project(proyecto_sha256)

set(CMAKE_CXX_STANDARD 17)

SET(GCC_COVERAGE_LINK_FLAGS    "-lssl -lcrypto")

#add_library(openssl openssl-1.1.1q)
#target_include_directories(openssl PUBLIC "./openssl-1.1.1q")
#set_target_properties(openssl PROPERTIES LINKER_LANGUAGE CXX)

add_executable(proyecto_sha256 main.cpp circular_list.h node.h list.h block.h entry.h BlockChain.h crypto.h main.cpp)
