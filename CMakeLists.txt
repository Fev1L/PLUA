cmake_minimum_required(VERSION 3.23)

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

#==============================================

add_executable(PLUA MACOSX_BUNDLE
        src/main.cpp
        src/Application.cpp
        src/Window.cpp
        src/ChatUI.cpp
        src/Database.cpp
        src/ModelRunner.cpp
        src/CharacterProfile.cpp
        src/PromptBuilder.cpp
        src/MemoryManager.cpp
        src/SessionManager.cpp
)

target_link_libraries(PLUA PRIVATE
        SDL3::SDL3
        imgui_lib
        OpenGL::GL
        sqlite3
)

target_include_directories(PLUA PRIVATE
        third_party/json
)