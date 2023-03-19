cmake_minimum_required(VERSION 3.14)

list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/mk")

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(SpeackerEngine, VERSION 0.1.0)

file(GLOB_RECURSE SPEACKER_INCLUDE CONFIGURE_DEPENDS "include/*")
file(GLOB_RECURSE SPEACKER_SOURCE CONFIGURE_DEPENDS "source/*")

add_executable(SpeackerEngine ${SPEACKER_SOURCE})
target_include_directories(SpeackerEngine PUBLIC ${SPEACKER_INCLUDE})