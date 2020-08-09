# create a target to build
add_executable(cameraMove cameraMove.cpp)

#
target_link_libraries(cameraMove PRIVATE OpenGL::GL GLEW::GLEW glfw learningOpenGL)

target_include_directories(cameraMove PRIVATE ../src)

#install(TARGETS appA DESTINATION bin)
