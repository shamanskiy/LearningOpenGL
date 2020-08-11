# create a target to build
add_executable(cameraMove cameraMove.cpp)

# link necessary external libraries and the project library
target_link_libraries(cameraMove PRIVATE OpenGL::GL GLEW::GLEW glfw learningOpenGL)

# provide paths to include files
target_include_directories(cameraMove PRIVATE "${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")
