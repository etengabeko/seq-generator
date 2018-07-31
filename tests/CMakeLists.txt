cmake_minimum_required(VERSION 2.8.12)
project(tests)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif (NOT CMAKE_BUILD_TYPE)

if (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-DNOEXCEPT=noexcept)
    add_compile_options(-std=c++11)
    add_compile_options(-Wall -Wextra -Werror -pedantic-errors)
endif (CMAKE_COMPILER_IS_GNUCXX)

if (MSVC)
    add_definitions("-DNOEXCEPT=throw()")
    add_compile_options(-DWIN32 -DWIN64 -DUNICODE -D_UNICODE)
endif (MSVC)

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/msg-debugger)
