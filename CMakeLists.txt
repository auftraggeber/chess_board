cmake_minimum_required(VERSION 3.25)
project(chess)

set(CMAKE_CXX_STANDARD 20)

Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.3.2)
FetchContent_MakeAvailable(Catch2)

add_executable(chess
        include/chess.h include/log.h include/math_utils.h
        src/main.cpp src/chess.cpp src/log.cpp src/math_utils.cpp include/common.h src/ui.cpp)

file(GLOB TEST_SOURCES tests/*.cpp tests/*.h)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
