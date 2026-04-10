cmake_minimum_required(VERSION 3.16)
project(BrickPong)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.0
)
FetchContent_MakeAvailable(raylib)

add_executable(BrickPong "brick pong.cpp")
target_link_libraries(BrickPong raylib)

# Windows: no console window popping up behind the game
if(WIN32)
  target_link_options(BrickPong PRIVATE -mwindows)
endif()
