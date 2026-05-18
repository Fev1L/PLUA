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

# =============== IMGUI ===============
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
target_link_libraries(imgui_lib PUBLIC SDL3::SDL3 OpenGL::GL)

# =============== SQLITE ===============
add_library(sqlite3 third_party/sqlite/sqlite3.c)
target_include_directories(sqlite3 PUBLIC third_party/sqlite)

# =============== LLAMA.CPP ===============
set(LLAMA_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LLAMA_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(LLAMA_BUILD_SERVER   OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/llama.cpp)

# =============== SOURCE FILES ===============
set(SOURCE_FILES
        src/main.cpp
        src/llama_backend.cpp
        src/ui.cpp
        src/chat_history.cpp
        src/load_config.cpp
)

# =============== ICONS ===============
if(APPLE)
    set(APP_ICON ${CMAKE_SOURCE_DIR}/assets/icon.icns)
    set_source_files_properties(${APP_ICON} PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources")
elseif(WIN32)
    configure_file(
            ${CMAKE_SOURCE_DIR}/assets/icon.rc.in
            ${CMAKE_BINARY_DIR}/icon.rc
    )
endif()

# =============== ADD EXECUTABLE ===============
if(APPLE)
    add_executable(PLUA MACOSX_BUNDLE
            ${SOURCE_FILES}
            ${APP_ICON}
            src/main.cpp
            src/chat_history.cpp
            src/load_config.cpp
            src/llama_backend.cpp
            src/ui.cpp
    )
    set_target_properties(PLUA PROPERTIES
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.fev1L.plua"
            MACOSX_BUNDLE_BUNDLE_NAME    "PLUA"
            MACOSX_BUNDLE_ICON_FILE      "icon.icns"
            MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
    )
elseif(WIN32)
    add_executable(PLUA WIN32
            ${SOURCE_FILES}
            ${CMAKE_BINARY_DIR}/icon.rc
            src/main.cpp
            src/chat_history.cpp
            src/load_config.cpp
            src/llama_backend.cpp
            src/ui.cpp
    )
else()
    add_executable(PLUA ${SOURCE_FILES})
endif()

# =============== LINK ===============
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

# =============== COPY FONTS ===============
if(APPLE)
    add_custom_command(TARGET PLUA POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/fonts
            $<TARGET_BUNDLE_CONTENT_DIR:PLUA>/Resources/fonts
            COMMENT "Copying fonts"
    )
else()
    add_custom_command(TARGET PLUA POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/fonts
            $<TARGET_FILE_DIR:PLUA>/fonts
            COMMENT "Copying fonts"
    )
endif()

# =============== COPY MODELS ===============
if(APPLE)
    add_custom_command(TARGET PLUA POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/models
            $<TARGET_BUNDLE_CONTENT_DIR:PLUA>/Resources/models
            COMMENT "Copying models"
    )
else()
    add_custom_command(TARGET PLUA POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/models
            $<TARGET_FILE_DIR:PLUA>/models
            COMMENT "Copying models"
    )
endif()