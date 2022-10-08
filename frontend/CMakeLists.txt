project(BlockChain C CXX)
cmake_minimum_required(VERSION 3.3.2)

include(cmake/App.cmake)

set(SOURCES "src/MyApp.h"
            "src/MyApp.cpp"
            "src/main.cpp")

find_package(OpenSSL REQUIRED)

add_executable(Backend
        ../backend/circular_list.h
        ../backend/node.h
        ../backend/list.h
        ../backend/block.h
        ../backend/entry.h
        ../backend/BlockChain.h
        ../backend/crypto.h
        ../backend/main.cpp)

target_link_libraries(Backend PRIVATE OpenSSL::SSL OpenSSL::Crypto)

target_include_directories(Backend PUBLIC "../backend")

target_link_directories(Backend PUBLIC "../backend")

add_app("${SOURCES}")

