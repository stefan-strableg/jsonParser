cmake_minimum_required(VERSION 3.28)

project(JsonParser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -static-libgcc -static-libstdc++)

add_library(libJsonParser
            inc/Array.hpp src/Array.cpp
            inc/Json.hpp
            inc/JsonEntity.hpp src/JsonEntity.cpp
            inc/Object.hpp src/Object.cpp
            inc/Raw.hpp
            inc/string.hpp src/string.cpp
            inc/Value.hpp src/Value.cpp)

add_executable(json-parser-test src/main.cpp)

target_link_libraries(json-parser-test PRIVATE libJsonParser)