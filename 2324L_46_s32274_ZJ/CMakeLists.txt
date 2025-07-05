cmake_minimum_required(VERSION 3.28)
project(2324L_46_s32274_ZJ)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS FALSE)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         10.2.1
)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY  https://github.com/SFML/SFML
        GIT_TAG         2.6.1
)

FetchContent_MakeAvailable(fmt)
FetchContent_MakeAvailable(SFML)

add_executable(2324L_46_s32274_ZJ Main.cpp
        WordObject.h
        WordObject.cpp)
target_link_libraries(2324L_46_s32274_ZJ fmt sfml-graphics sfml-audio)

