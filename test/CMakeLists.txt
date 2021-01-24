# unit_tests is a single executable that runs tests
# in all listed .cpp files
add_executable(
    unit_tests
    tests.cpp
    )

target_link_libraries(
    unit_tests
    GTest::gtest_main
    )
