cmake_minimum_required(VERSION 3.10)
project(rtls)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # Output directory for static lib (.lib/.a)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for shared lib (.dll/.so)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for executables (.exe)

if( MSVC )
    add_compile_options(/MP) # Enable multithreaded builds
    add_definitions(-D_CRT_SECURE_NO_WARNINGS) # Disable warning about scanf
endif()

add_subdirectory(thirdparty/serial)
add_subdirectory(thirdparty/dwm)
add_subdirectory(thirdparty/ixwebsocket)
add_subdirectory(thirdparty/abelectronics)

if(NOT MSVC)
    add_subdirectory(thirdparty/abelectronics) # Linux dependent
endif()

add_subdirectory(src)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest) # Include the GoogleTest module to enable gtest_discover_tests()
    set(gtest_force_shared_crt ON)
    add_subdirectory(thirdparty/googletest)
    add_subdirectory(tests)
endif()
