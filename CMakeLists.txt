cmake_minimum_required(VERSION 3.10)

project(snake_game)

set(CMAKE_CXX_STANDARD 11)

add_executable(snake_game main.cpp)

if(MSVC)
  target_compile_options(snake_game PRIVATE /W4)
endif()
# 添加链接库
target_link_libraries(snake_game ncurses)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(snake_game PRIVATE -Wall -Wextra -pedantic)
endif()