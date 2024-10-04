cmake_minimum_required(VERSION 3.26.5)
project(LiveGame LANGUAGES CXX)

# set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

add_subdirectory(SFML)
# set SFML directory
set(SFML_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/SFML/build/lib/Debug")

# include head files
include_directories(include)

# Find OpenMP
find_package(OpenMP REQUIRED)

# source files
set(SOURCES
    code/Main.cpp
    code/Globals.cpp
    code/Block.cpp
    code/Grid.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Link OpenMP libraries
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()


if(WIN32)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_LIBRARY_DIR}/sfml-system-d-2.dll"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/sfml-system-d-2.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_LIBRARY_DIR}/sfml-window-d-2.dll"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/sfml-window-d-2.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_LIBRARY_DIR}/sfml-graphics-d-2.dll"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/sfml-graphics-d-2.dll"
    COMMENT "Copying SFML DLLs to output directory")
endif()
