add_executable(firstTriangle
    firstTriangle.cpp
    loadShader.cpp)
target_link_libraries(firstTriangle PUBLIC GLEW::GLEW glfw)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/vertexShader.glsl" "${CMAKE_CURRENT_BINARY_DIR}/" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/fragmentShader.glsl" "${CMAKE_CURRENT_BINARY_DIR}/" COPYONLY)