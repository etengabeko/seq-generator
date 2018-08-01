cmake_minimum_required(VERSION 2.8.12)
project(sequence-generator-test)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif (NOT CMAKE_BUILD_TYPE)

if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++11)
    add_compile_options(-Wall -Wextra -Werror -pedantic-errors)
endif (CMAKE_COMPILER_IS_GNUCXX)

if (MSVC)
    add_compile_options(-DWIN32 -DWIN64 -DUNICODE -D_UNICODE)
endif (MSVC)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(ROOT_PROJECT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${ROOT_PROJECT_SRC_DIR})

set(ROOT_PROJECT_HEADERS
    ${ROOT_PROJECT_SRC_DIR}/base_generator.h
    ${ROOT_PROJECT_SRC_DIR}/thread_safe_generator.h
)

set(HEADERS
    ${ROOT_PROJECT_HEADERS}
)

set(SOURCES
    src/sequence_generator_test.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

if (CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(${PROJECT_NAME} pthread)
endif (CMAKE_COMPILER_IS_GNUCXX)

add_test(${PROJECT_NAME} ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/sbin"
)
