# LearningOpenGL
LearningOpenGL is a small beginner-level project to learn a bit of OpenGL.

The project follows the guidelines of modern OpenGL and demonstrates the complete rendering pipeline, object importing, shading, interactive camera, texturing and more. This is also my playground to experiment with Cmake, vcpkg and unit testing.

<img src="https://raw.githubusercontent.com/Shamanskiy/LearningOpenGL/media/images/hero_image.png" width="500">

## Prerequisites
* CMake >= 3.20
* C++ compiler (MSVC, GCC, Clang)

## How to configure
This project uses vcpkg to manage dependencies. If you don't have [https://vcpkg.io/en/index.html](aass)
Use CMake to configure the project. For example, from the project root directory do
```
mkdir build
cd build
cmake ..
```
When you configure the project for the first time, vcpkg will download OpenGL, GLEW, glfw3, glm, assimp and googletest (and possibly other libraries, too) and install them locally in the CMake build folder. This step may take a while. 

 ## How to test
 The `unit_tests` target builds all the unit tests. Build it with
 ```
 cmake --build . --target unit_tests --config Release
 ```
 or using your preferred method.

 ## How to run
 The `exampleScene` target builds the demo app. Build it with
 ```
 cmake --build . --target exampleScene --config Release
 ```
 or using your preferred method. Enjoy!

 cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/Users/shamanskiy/Software/vcpkg/scripts/buildsystems/vcpkg.cmake


