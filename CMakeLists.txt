cmake_minimum_required(VERSION 3.22)

project(sha256)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp circular_list.h node.h list.h block.h entry.h blockchain.h crypto.h transactionentry.h studententry.h)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
