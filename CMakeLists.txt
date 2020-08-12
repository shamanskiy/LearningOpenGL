cmake_minimum_required(VERSION 3.10)

project(LearningOpenGL
      VERSION 1.0
      DESCRIPTION "OpenGL exercises following the OpenGL tutorial by Ben Cook from Udemy.com"
      LANGUAGES CXX)

# Let's ensure -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_EXTENSIONS OFF)

# Let's nicely support folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# find necessary libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(bin)

# configure Config.h to define
# LEARNING_OPENGL_SOURCE_PATH
# LEARNING_OPENGL_VERSION_MAJOR
# LEARNING_OPENGL_VERSION_MINOR
configure_file("${PROJECT_SOURCE_DIR}/src/Config.h.in" "${PROJECT_BINARY_DIR}/src/Config.h")
