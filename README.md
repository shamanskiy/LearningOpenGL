# LearningOpenGL
LearningOpenGL is a small beginner-level project to learn a bit of OpenGL.

The project follows the guidelines of modern OpenGL and demonstrates the complete rendering pipeline, object importing, shading, interactive camera, texturing and more. This is also my playground to experiment with Cmake, vcpkg and unit testing.

<img src="https://raw.githubusercontent.com/Shamanskiy/LearningOpenGL/media/images/hero.png" width="500">

## Prerequisites
* CMake >= 3.20
* vcpkg
* C++ compiler (MSVC, GCC, Clang)

## How to get vcpkg
This project uses [vcpkg](https://vcpkg.io/en/index.html) to manage dependencies. If you don't have vcpkg, you can get it by running
```
git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg
```

## How to configure the project
Use CMake to configure the project. For example, from the project root directory do
```
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```
The CMAKE_TOOLCHAIN_FILE settings allows CMake to use libraries provided by vcpkg. If anything is missing, vcpkg will download and build it. When you configure the project for the first time, vcpkg will download OpenGL, GLEW, glfw3, glm, assimp and googletest (and possibly other libraries, too). This step may take a while. 

 ## How to test
 The `unit_tests` target builds all the unit tests. Build it with
 ```
 cmake --build . --target unit_tests
 ```
 or using your preferred method.

 ## How to run
 The `exampleScene` target builds the demo app. Build it with
 ```
 cmake --build . --target exampleScene
 ```
 or using your preferred method. Enjoy!


