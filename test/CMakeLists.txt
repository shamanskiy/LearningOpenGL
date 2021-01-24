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

# This is probably unnecessary. Target_link_libraries
# should provide all necessary headers
#target_include_directories(unit_tests PRIVATE ${GTEST_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/#src)

