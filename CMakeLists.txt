cmake_minimum_required(VERSION 3.22)

project(proyecto_sha256)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)

add_executable(proyecto_sha256 main.cpp circular_list.h node.h list.h block.h entry.h BlockChain.h crypto.h main.cpp)

target_link_libraries(proyecto_sha256 PRIVATE OpenSSL::SSL OpenSSL::Crypto)
