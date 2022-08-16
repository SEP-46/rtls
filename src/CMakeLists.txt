set(SRCS
    lmh_serial.cpp
    rtls.cpp
    rtls.h
    trilaterationsolver.h
    trilaterationsolver_basic.cpp
    trilaterationsolver_basic.h
    vector.h
)

add_library(${PROJECT_NAME} ${SRCS})
add_executable(rtls-main main.cpp)
target_link_libraries(rtls-main ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} serial)
target_link_libraries(${PROJECT_NAME} dwm)