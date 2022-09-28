set(SRCS
    analog_interface.h
    analog_interface.cpp
    communication_interface.h
    lmh_serial.cpp
    lmh_serial.h
    mathlib.h
    mock_tag.cpp
    mock_tag.h
    optparse.h
    rtls.cpp
    rtls.h
    settings.cpp
    settings.h
    tag.h
    trilaterationsolver.h
    trilaterationsolver_basic.cpp
    trilaterationsolver_basic.h
    uart_interface.cpp
    uart_interface.h
    util.cpp
    util.h
    uwb_anchor.cpp
    uwb_anchor.h
    uwb_tag.cpp
    uwb_tag.h
    vector.h
    velocity_output.cpp
    velocity_output.h
    websocket_interface.cpp
    websocket_interface.h
)

add_library(${PROJECT_NAME} ${SRCS})
add_executable(rtls-main main.cpp)
target_link_libraries(rtls-main ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} serial)
target_link_libraries(${PROJECT_NAME} dwm)
target_link_libraries(${PROJECT_NAME} ixwebsocket)

if(NOT MSVC)
    target_link_libraries(${PROJECT_NAME} abelectronics) # Linux Dependent
endif()
