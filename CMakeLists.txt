cmake_minimum_required(VERSION 3.10)
project(Shader)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect source files
file(GLOB_RECURSE SOURCES 
    src/*.cpp
    src/*.c
    src/*.hpp
    src/*.h
    src/shader/*.glsl
)

set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/include/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/include/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/include/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/include/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/include/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/include/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/include/imgui/backends/imgui_impl_opengl3.cpp
)

# Create executable
add_executable(ZShader ${SOURCES} ${IMGUI_SOURCES})

# Include directories
target_include_directories(ZShader PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/imgui
    ${CMAKE_SOURCE_DIR}/include/imgui/backends
)

# Link libraries
file(GLOB LIBRARIES "${CMAKE_SOURCE_DIR}/lib/*.lib")
target_link_directories(ZShader PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(ZShader PRIVATE ${LIBRARIES})

# Define directories for resources
set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shaders)
set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)
set(DLL_DIR ${CMAKE_SOURCE_DIR}/dlls)

# Post-build commands to copy resources
add_custom_command(TARGET ZShader POST_BUILD
    COMMENT "Copying runtime files..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${DLL_DIR} $<TARGET_FILE_DIR:ZShader>
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SHADER_DIR} $<TARGET_FILE_DIR:ZShader>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_DIR} $<TARGET_FILE_DIR:ZShader>/assets
)

# Define source directory macro
target_compile_definitions(ZShader PRIVATE
    -DSOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

# Configure compile options based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(ZShader PRIVATE -g -O0)
else()
    target_compile_options(ZShader PRIVATE -O3)
endif()

# Organize files into groups
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SOURCES} ${IMGUI_SOURCES})