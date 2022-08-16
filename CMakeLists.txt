cmake_minimum_required(VERSION 3.10)
project(rtls)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # Output directory for static lib (.lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for shared lib (.dll)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for executables (.exe)

if( MSVC )
    add_compile_options(/MP) # Enable multithreaded builds
    add_definitions(-D_CRT_SECURE_NO_WARNINGS) # Disable warning about scanf
endif()

add_subdirectory(thirdparty/serial)

set(SRCS
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} serial)