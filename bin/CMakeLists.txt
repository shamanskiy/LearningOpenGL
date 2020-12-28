# create a target to build
add_executable(exampleScene exampleScene.cpp)

# link necessary external libraries and the project library
target_link_libraries(exampleScene PRIVATE ${assimp_LIBRARIES} OpenGL::GL GLEW::GLEW glfw learningOpenGL)

# provide paths to include files
target_include_directories(exampleScene PRIVATE "${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")
