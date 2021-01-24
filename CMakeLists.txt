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

# find necessary libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED) # a distribution/wrapper for OpenGL (I guess)
find_package(glfw3 REQUIRED) # window and IO manager for OpenGL
find_package(glm REQUIRED) # linear algebra library for OpenGL
find_package(GTest REQUIRED) # unit_testing

# we use CONFIG mode to find ASSIMP because we want to use a config file coming with ASSIMP
find_package(ASSIMP CONFIG REQUIRED)
if(ASSIMP_FOUND)
    set(ASSIMP_LIBNAME ${ASSIMP_LIBRARY_DIRS}/${ASSIMP_LIBRARIES})
    message("-- Found assimp: ${ASSIMP_LIBNAME}")
endif(ASSIMP_FOUND)


find_path(stb_image_INCLUDE_DIR stb_image.h REQUIRED)
include_directories(${stb_image_INCLUDE_DIR})



# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(bin)

# Unit tests are here
add_subdirectory(test)

# configure Config.h to set some #defines
configure_file("${PROJECT_SOURCE_DIR}/src/Config.h.in" "${PROJECT_BINARY_DIR}/src/Config.h")


####################
# fetch content for textures
