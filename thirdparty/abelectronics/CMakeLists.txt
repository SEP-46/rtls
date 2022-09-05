cmake_minimum_required(VERSION 3.10)
project(abelectronics)

## Sources
set(abelectronics_SRCS
    src/ABE_ADCDACPi.cpp
    include/ABE_ADCDACPi.h)

## Add abelectronics library
add_library(${PROJECT_NAME} ${abelectronics_SRCS})

## Include headers
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
