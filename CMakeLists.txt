cmake_minimum_required(VERSION 3.14)

list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/makeFind")

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

message(STATUS "Source dir: ${CMAKE_SOURCE_DIR}")

project(SpeackerEngine, VERSION 0.1.0)

include_directories("include/")

file(GLOB_RECURSE SPEACKER_SOURCE CONFIGURE_DEPENDS "source/*.cpp")

find_package(OpenGL REQUIRED)

find_package(glfw3 3.3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(SOIL REQUIRED)
include_directories(${SOIL_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIR})

find_package(LibVorbisfile REQUIRED)
include_directories(${VORBIS_INCLUDE_DIR})

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

add_library(SpeackerEngine SHARED ${SPEACKER_SOURCE})

target_link_libraries(SpeackerEngine ${GLEW_LIBRARIES})
target_link_libraries(SpeackerEngine ${OPENGL_LIBRARY})
target_link_libraries(SpeackerEngine ${GLFW_LIBRARY})
target_link_libraries(SpeackerEngine ${SOIL_LIBRARY})
target_link_libraries(SpeackerEngine ${CURL_LIBRARIES})
target_link_libraries(SpeackerEngine ${OPENAL_LIBRARY})
target_link_libraries(SpeackerEngine ${VORBISFILE_LIBRARY})
target_link_libraries(SpeackerEngine ${CURL_LIBRARY})

install(TARGETS SpeackerEngine DESTINATION lib)
