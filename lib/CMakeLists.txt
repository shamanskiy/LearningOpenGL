# define lists of header, source and shader files for convenience
set(HEADERS_LIST
  ${PROJECT_SOURCE_DIR}/include/RendGL/Camera.h
  ${PROJECT_SOURCE_DIR}/include/RendGL/Light.h
  ${PROJECT_SOURCE_DIR}/include/RendGL/Model.h
  ${PROJECT_SOURCE_DIR}/include/RendGL/Scene.h
  ${PROJECT_SOURCE_DIR}/include/RendGL/Shader.h
  ${PROJECT_SOURCE_DIR}/include/RendGL/Utils.h
  ${PROJECT_SOURCE_DIR}/include/RendGL/Window.h
)

set(SOURCES_LIST
  ${PROJECT_SOURCE_DIR}/lib/Camera.cpp
  ${PROJECT_SOURCE_DIR}/lib/Light.cpp
  ${PROJECT_SOURCE_DIR}/lib/Model.cpp
  ${PROJECT_SOURCE_DIR}/lib/Scene.cpp
  ${PROJECT_SOURCE_DIR}/lib/Shader.cpp
  ${PROJECT_SOURCE_DIR}/lib/Utils.cpp
  ${PROJECT_SOURCE_DIR}/lib/Window.cpp
)

set(SHADERS_LIST
  ${PROJECT_SOURCE_DIR}/lib/shaders/fragmentShader.glsl
  ${PROJECT_SOURCE_DIR}/lib/shaders/vertexShader.glsl
)

set(SCENES_LIST
  ${PROJECT_SOURCE_DIR}/scenes/exampleScene.json
  ${PROJECT_SOURCE_DIR}/scenes/welcomeToOpenGL_hero.json
)

# define a library to compile
add_library(RendGL
  ${SOURCES_LIST}
  # header files are not necessary here. Only adding them so that
  # they are visible in an IDE
  ${HEADERS_LIST}
  # same for the shaders
  ${SHADERS_LIST}
)

# link necessary external libraries
# PUBLIC means that they will also be linked to any downstream app or lib
target_link_libraries(RendGL
  PUBLIC
    GLEW::GLEW
    glfw
    glm::glm
    assimp::assimp
)

# require c++14 for the library and anything that depends on it
target_compile_features(RendGL PUBLIC cxx_std_17)

# add include directories here so that they available in anything
# that depends on RendGL
target_include_directories(RendGL
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include/RendGL
    ${PROJECT_SOURCE_DIR}/include/extern
    ${PROJECT_BINARY_DIR}/include
)

# shaders will appear in a Shaders folder in an IDE
# for header and source files this happens automatically
source_group(Shaders
  FILES
    ${SHADERS_LIST}
)

source_group(Scenes
  FILES
    ${SCENES_LIST}
)
