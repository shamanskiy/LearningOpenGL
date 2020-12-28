cmake_minimum_required(VERSION 3.10)

project(LearningOpenGL
      VERSION 1.0
      DESCRIPTION "OpenGL exercises following the OpenGL tutorial by Ben Cook from Udemy.com"
      LANGUAGES CXX)

# we want to use c++14      
set (CMAKE_CXX_STANDARD 14)

# Let's ensure -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_EXTENSIONS OFF)

# Let's nicely support folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# we want to find external packages not supported by Cmake by default.
# Find***.cmake files for each such package are in cmake/Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# find necessary libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

find_path(glm_INCLUDE_DIR glm/glm.hpp REQUIRED)
include_directories(${glm_INCLUDE_DIR})

find_path(stb_image_INCLUDE_DIR stb_image.h REQUIRED)
include_directories(${stb_image_INCLUDE_DIR})

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(bin)

# configure Config.h to define
# LEARNING_OPENGL_SOURCE_PATH
# LEARNING_OPENGL_VERSION_MAJOR
# LEARNING_OPENGL_VERSION_MINOR
configure_file("${PROJECT_SOURCE_DIR}/src/Config.h.in" "${PROJECT_BINARY_DIR}/src/Config.h")

