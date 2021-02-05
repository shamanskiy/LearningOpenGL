cmake_minimum_required(VERSION 3.18)

project(LearningOpenGL
      VERSION 1.0
      DESCRIPTION "OpenGL exercises following the OpenGL tutorial by Ben Cook from Udemy.com"
      LANGUAGES CXX)

# We do not want to use GNU extension of C++ (may be unavailable on some platforms)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use folders to structure the project in IDEs that support folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# find necessary libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED) # a distribution/wrapper for OpenGL (I guess)
find_package(glfw3 REQUIRED) # window and IO manager for OpenGL
find_package(glm CONFIG REQUIRED) # linear algebra library for OpenGL
find_package(assimp CONFIG REQUIRED) # asset import
find_package(GTest CONFIG REQUIRED) # unit_testing


message("yoyo")

# download content (textures and models) from AWS S3 if not yet downloaded
include(cmake/DownloadData.txt)

# The compiled library code is here
add_subdirectory(lib)

# The executable code is here
add_subdirectory(app)

# Unit tests are here
add_subdirectory(test)

# Config.h contains various useful #defines:
# TEXTURE_DIR is a path to textures.
# SHADERS_DIR is a path to shaders.
set(SHADERS_DIR ${PROJECT_SOURCE_DIR}/lib/shaders)
configure_file("${PROJECT_SOURCE_DIR}/lib/Config.h.in" "${PROJECT_BINARY_DIR}/include/Config.h")



