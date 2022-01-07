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

# The compiled library code is here
add_subdirectory(lib)

# The executable code is here
add_subdirectory(apps)

# Unit tests are here
add_subdirectory(tests)

# Configure path macros
include(cmake/configure.cmake)


