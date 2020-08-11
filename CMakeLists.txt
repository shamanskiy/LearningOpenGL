cmake_minimum_required(VERSION 3.10)

project(LearningOpenGL
      VERSION 1.0
      DESCRIPTION "OpenGL exercises following the OpenGL tutorial by Ben Cook from Udemy.com"
      LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Testing only available if this is the main app
#    include(CTest)

    # Docs only available if this is the main app
    #find_package(Doxygen)
    #if(Doxygen_FOUND)
    #  add_subdirectory(docs)
  #else()
  #    message(STATUS "Doxygen not found, not building docs")
#  endif()

endif()

# find necessary libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(bin)

# define a variable with the path to shaders; it will be configured in Config.h and made available in the code as LEARNING_OPENGL_SHADERS_DIR
set(SHADERS_DIR "${PROJECT_SOURCE_DIR}/shaders")

# configure Config.h to define
# LEARNING_OPENGL_SHADERS_DIR
# LEARNING_OPENGL_VERSION_MAJOR
# LEARNING_OPENGL_VERSION_MINOR
configure_file("${PROJECT_SOURCE_DIR}/src/Config.h.in" "${PROJECT_BINARY_DIR}/src/Config.h")
