cmake_minimum_required(VERSION 3.27)
project(projektPJC)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS FALSE)
include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY
        https://github.com/SFML/SFML
        GIT_TAG
        2.6.1
)

FetchContent_MakeAvailable(SFML)

add_executable(projektPJC
        main.cpp
        monkeyutils.cpp
        monkeyutils.h
)
target_link_libraries(projektPJC sfml-graphics)
