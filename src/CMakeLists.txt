# define a library to compile
add_library(learningOpenGL Camera.cpp Mesh.cpp Texture.cpp Window.cpp Shader.cpp Light.cpp)

# link necessary external libraries
target_link_libraries(learningOpenGL PUBLIC OpenGL::GL GLEW::GLEW glfw)
