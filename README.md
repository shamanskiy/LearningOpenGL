# LearningOpenGL
LearningOpenGL is a small beginner-level project to learn a bit of OpenGL.

The project follows the guidelines of modern OpenGL and demonstrates the complete rendering pipeline, object importing, shading, interactive camera, texturing and more. This is also my playground to experiment with Cmake, vcpkg and unit testing.

<img src="https://raw.githubusercontent.com/Shamanskiy/LearningOpenGL/media/images/hero_image.png" width="500">

## How to download
The project includes [vcpkg](https://github.com/microsoft/vcpkg) as a submodule. To clone everything in one go, do
```
git clone --recurse-submodules https://github.com/Shamanskiy/LearningOpenGL.git
```
If you have already cloned the project and CMake complains that it can't find vcpkg, you have to clone the submodule yourself. To that end, do
```
git submodule init
git submodule update
```

## How to configure
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


