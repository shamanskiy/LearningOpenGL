# create a target to build
add_executable(cameraMove main.cpp)

#
target_link_libraries(cameraMove PRIVATE ${OpenGL_LIBRARY} GLEW::GLEW glfw)

target_include_directories(cameraMove PRIVATE /usr/local/include)

#install(TARGETS appA DESTINATION bin)
