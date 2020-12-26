# define a library to compile
add_library(learningOpenGL
    Camera.cpp
    Light.cpp
    Mesh.cpp
    Primitive.cpp
    Shader.cpp
    StaticModel.cpp
    Texture.cpp
    Window.cpp
    
    # add header files so that they are visible from an IDE
    Camera.h
    Light.h
    Mesh.h
    Primitive.h
    Shader.h
    StaticModel.h
    Texture.h
    Window.h
    # same for the shaders
    shaders/fragmentShader_texture.glsl
    shaders/fragmentShader_noTexture.glsl
    shaders/vertexShader_texture.glsl
    shaders/vertexShader_noTexture.glsl)

# link necessary external libraries
target_link_libraries(learningOpenGL PUBLIC OpenGL::GL GLEW::GLEW glfw)

