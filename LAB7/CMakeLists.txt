cmake_minimum_required(VERSION 2.8)
find_library(PTHREAD_LIBRARY pthread)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set_source_files_properties(LAB7.c PROPERTIES LANGUAGE CXX )
add_executable(program LAB7.c)
target_link_libraries(program ${PTHREAD_LIBRARY})