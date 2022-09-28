set(TESTS
    analog_test.cpp
    optparse_test.cpp
    trilaterationbasic_test.cpp
    uwb_anchor_test.cpp
    vector_test.cpp
    velocity_test.cpp
)

add_executable(rtls-tests ${TESTS})

include_directories(../src)

# link the Google test infrastructure and a default main function to the test executable.
# Remove g_test_main if writing your own main function.
target_link_libraries(rtls-tests gtest gtest_main ${PROJECT_NAME})

gtest_discover_tests(rtls-tests
    # set working directory to your project root so that you can find test data via paths relative to the project root
    WORKING_DIRECTORY ${PROJECT_DIR}
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
)

set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)