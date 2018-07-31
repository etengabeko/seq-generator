cmake_minimum_required(VERSION 2.8.12)
project(sequence-generator)

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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES
    src/main.cpp
)

set(HEADERS
    src/base_generator.h
    src/thread_safe_generator.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

if (CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(${PROJECT_NAME} pthread)
endif (CMAKE_COMPILER_IS_GNUCXX)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
)
