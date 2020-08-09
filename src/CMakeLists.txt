add_library(learningOpenGL Camera.cpp Mesh.cpp Texture.cpp Window.cpp Shader.cpp)

target_link_libraries(learningOpenGL PUBLIC OpenGL::GL GLEW::GLEW glfw)
