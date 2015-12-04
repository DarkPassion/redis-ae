cmake_minimum_required(VERSION 2.8)

PROJECT(redis-ae${MYVER})
set (CMAKE_BUILD_TYPE DEBUG)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
#file(GLOB_RECURSE source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.c")


#add_library(redis-ae${MYVER} STATIC ${source_files})
add_library(redis-ae${MYVER} STATIC ae.c anet.c zmalloc.c)


