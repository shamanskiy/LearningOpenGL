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
target_link_libraries(learningOpenGL PUBLIC OpenGL::GL GLEW::GLEW glfw glm::glm)

# require c++14 for the library and anything that depends on it
target_compile_features(learningOpenGL PUBLIC cxx_std_14)

# include external header-only libs (currently, only std_image.h)
target_include_directories(learningOpenGL PUBLIC ${PROJECT_SOURCE_DIR}/include/extern)

