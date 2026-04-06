cmake_minimum_required(VERSION 3.23)

project(PLUA)

include(cmake/Dependencies.cmake)

find_package(OpenGL REQUIRED)

add_library(imgui_lib
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp

        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_lib PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui_lib PUBLIC
        SDL3::SDL3
        OpenGL::GL
)

add_executable(PLUA MACOSX_BUNDLE
    main.cpp
        types.h
)

target_link_libraries(PLUA PRIVATE
        SDL3::SDL3
        imgui_lib
        OpenGL::GL
)