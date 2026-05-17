set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_OBJC_COMPILER /usr/bin/clang)
enable_language(OBJC)

cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(PLUA)

include(cmake/Dependencies.cmake)

find_package(OpenGL REQUIRED)

#=============== IMGUI CONNECT ===============

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

#=============== SQLITE CONNECT ===============

add_library(sqlite3 third_party/sqlite/sqlite3.c)

target_include_directories(sqlite3 PUBLIC third_party/sqlite)

# =============== LLAMA.CPP CONNECT ===============

set(LLAMA_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LLAMA_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LLAMA_BUILD_SERVER OFF CACHE BOOL "" FORCE)

add_subdirectory(third_party/llama.cpp)

# ===============================================

add_executable(PLUA MACOSX_BUNDLE
        src/main.cpp
        src/llama_backend.cpp
        src/ui.cpp
        src/chat_history.cpp
        src/load_config.cpp
)

target_link_libraries(PLUA PRIVATE
        SDL3::SDL3
        imgui_lib
        OpenGL::GL
        sqlite3
        llama
)

target_include_directories(PLUA PRIVATE
        third_party/json
        third_party/llama.cpp/include
)