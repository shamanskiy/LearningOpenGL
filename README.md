# LearningOpenGL
LearningOpenGL is a small project based on [a great OpenGL course by Ben Cook on Udemy.com.](https://www.udemy.com/course/graphics-with-modern-opengl)

The project follows the guidelines of modern OpenGL and demonstrates the complete rendering pipeline, object importing, shading, interactive camera, texturing and more. This is also my playground to experiment with Cmake and unit testing.

## Dependencies
The following libraries have to be installed to compile the project:
* OpenGL
* [GLEW](http://glew.sourceforge.net/) - for managing OpenGL extensions
* [glfw3](https://www.glfw.org/) - for window and input management
* [glm](https://glm.g-truc.net/0.9.9/index.html) - for linear algebra (header-only)
* assimp
* gtest

## How to build

#### Ubuntu
Installing dependencies on Ubuntu is a piece of cake:
```
sudo
apt-get update
apt-get install libglew-dev
apt-get install libglfw3
apt-get install libglfw3-dev
apt-get install libglm-dev
```
After that, execute the following commands from the project directory:
```
mkdir build
cd build
cmake .. 
make
```
Nice and easy.
#### MacOS
Building on MacOS is very similar to building on Ubuntu. One of differences is that you use [brew](https://brew.sh/) instead of `apt` or `apt-get` to install dependencies:
```
brew install glew
brew install glfw3
brew install glm
```
For stb_image, download the [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h) file and put it into `/usr/local/include`. After that, execute the following commands from the project directory:
```
mkdir build
cd build
cmake ..
make
```
Alternatively, use can configure the project for Xcode:
```
cmake .. -G "Xcode"
```
Then, open and build the project in Xcode.

P.S. Make sure to update the [Command Line Tools](developer.apple.com/download/more) or you may run into troubles with linking the OpenGL framework.

#### Windows
You can use [vcpkg](https://github.com/microsoft/vcpkg) to install all necessary dependencies:
```
./vcpkg install GLEW:x64-windows
./vcpkg install glfw3:x64-windows
./vcpkg install glm:x64-windows
./vcpkg install stb:x64-windows
```
Then, use cmake or CMake-gui to configure the project for Visual Studio. Add variable `CMAKE_TOOLCHAIN_FILE` and set it to `path/to/vcpkg/scripts/buildsystems/vcpkg.cmake` - this will allow CMake to easily find all libraries installed with `vcpkg`. Open and build the project in Visual Studio.
