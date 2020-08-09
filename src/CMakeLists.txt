add_library(learningOpenGL Camera.cpp Mesh.cpp Texture.cpp Window.cpp)

target_link_libraries(learningOpenGL PUBLIC OpenGL)
