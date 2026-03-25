cmake_minimum_required(VERSION 3.23)

project(PLUA)

include(cmake/Dependencies.cmake)

add_executable(PLUA MACOSX_BUNDLE
    main.cpp
        types.h
)

target_link_libraries(PLUA PRIVATE
        SDL3::SDL3
)