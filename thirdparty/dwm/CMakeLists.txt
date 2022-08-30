cmake_minimum_required(VERSION 3.10)
project(dwm)

## Sources
set(dwm_SRCS
    src/dwm_api.c
    src/lmh.c
    include/dwm_api.h
    include/dwm1001_tlv.h
    include/lmh.h
)

## Add DWM library
add_library(${PROJECT_NAME} ${dwm_SRCS})

## Include headers
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
