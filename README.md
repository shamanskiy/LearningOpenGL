# LearningOpenGL
LearningOpenGL is a small beginner-level project to learn a bit of OpenGL.

The project follows the guidelines of modern OpenGL and demonstrates the complete rendering pipeline, object importing, shading, interactive camera, texturing and more. This is also my playground to experiment with Cmake and unit testing.

<img src="https://raw.githubusercontent.com/Shamanskiy/LearningOpenGL/media/images/hero_image.png" width="500">

## How to configure
Use CMake to configure the project. For example, from the project root directory do
```
mkdir build
cd build
cmake ..
```
 The project uses [vcpkg](https://github.com/microsoft/vcpkg) to manage dependencies. When you configure the project for the first time, vcpkg will download OpenGL, GLEW, glfw3, glm, assimp and googletest and install them locally in the the CMake build folder. This step may take a while. After that, build the project using the tool you've configured the project for. On Linux and MacOS, you can just use
 ```
 make
 ```
 On Windows with Visual Studio, CMake will create a VS solution file. Use the file to open and build the project in Visual Studio.

 ## How to test
 Build and run the `unit_tests` target.

 ## How to run
 Build and run the `example_scene` target. Enjoy!


