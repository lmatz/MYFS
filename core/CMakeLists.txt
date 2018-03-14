cmake_minimum_required(VERSION 3.5)
include_directories(".")
set(CMAKE_CXX_STANDARD 14)


include_directories(.)

file(GLOB_RECURSE CoreSources src/*.cpp)
add_library(LibMyfsCore STATIC ${CoreSources})
