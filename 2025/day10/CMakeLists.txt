cmake_minimum_required(VERSION 4.0)
project(day10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# find_package(scn)
include(FetchContent)

# Define the dependency
FetchContent_Declare(
    scn
    GIT_REPOSITORY https://github.com/eliaskosunen/scnlib.git
    GIT_TAG        v4.0.1
)

FetchContent_MakeAvailable(scn)

add_executable(part1 part1.cpp)
add_executable(part2 part2.cpp)


target_link_libraries(part2 PRIVATE scn::scn)
target_link_libraries(part1 PRIVATE scn::scn)