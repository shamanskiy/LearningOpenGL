# Config.h contains useful macros that help executables find necessary files:

set(SHADERS_DIR ${PROJECT_SOURCE_DIR}/lib/shaders)
set(SCENES_DIR ${PROJECT_SOURCE_DIR}/scenes)
set(TEXTURES_DIR ${PROJECT_SOURCE_DIR}/assets/textures)
set(MODELS_DIR ${PROJECT_SOURCE_DIR}/assets/models)

configure_file("${PROJECT_SOURCE_DIR}/lib/Config.h.in" "${PROJECT_BINARY_DIR}/include/Config.h")


