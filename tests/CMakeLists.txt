# unit_tests is a single executable that runs tests in all listed .cpp files
add_executable(unit_tests
  CameraTest.cpp
  UtilsTest.cpp
)

# gtest_main is a standard main file to launch the unit_test app.
# if we wanted to write a custom one, we wouldn't link against gtest_main
target_link_libraries(unit_tests
  GTest::gtest_main
  RendGL
)
