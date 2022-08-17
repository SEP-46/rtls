set(SRCS
    lmh_serial.cpp
    rtls.cpp
    rtls.h
    trilaterationsolver.h
    trilaterationsolver_basic.cpp
    trilaterationsolver_basic.h
    uwb_anchor.cpp
    uwb_anchor.h
    uwb_tag.cpp
    uwb_tag.h
    vector.h
)

add_library(${PROJECT_NAME} ${SRCS})
add_executable(rtls-main main.cpp)
target_link_libraries(rtls-main ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} serial)
target_link_libraries(${PROJECT_NAME} dwm)