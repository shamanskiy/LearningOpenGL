add_executable(
    unit_tests
    tests.cpp
    )

target_link_libraries(
    unit_tests
    GTest::gtest_main
    )

#target_include_directories(unit_tests PRIVATE ${GTEST_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/#src)


add_test(
  NAME
    unit
  COMMAND
    ${CMAKE_BINARY_DIR}/bin/unit_tests
  )
