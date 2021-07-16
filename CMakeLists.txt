cmake_minimum_required(VERSION 3.2)
project(PMR_Alloc_Examples)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(PmrExample1)
add_subdirectory(PmrExample2)
add_subdirectory(PmrExample4)
add_subdirectory(PmrExample5)