# LearningOpenGL
LearningOpenGL is a small project based on [a great OpenGL course by Ben Cook on Udemy.com.](https://www.udemy.com/course/graphics-with-modern-opengl)

The project follows the guidelines of modern OpenGL and demonstrates the complete rendering pipeline, object importing, shading, interactive camera, texturing and more.

## Dependencies
The following libraries have to be installed to compile the project:
* OpenGL
* [GLEW](http://glew.sourceforge.net/) - for managing OpenGL extensions
* [glfw3](https://www.glfw.org/) - for window and input management
* [glm](https://glm.g-truc.net/0.9.9/index.html) - for linear algebra (header-only)
* [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) - for texture loading (header-only, part of [stb](https://github.com/nothings/stb))

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
For stb_image, download the [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h) file and put it into `/usr/local/include`.
After that, execute the following commands from the project directory:
```
mkdir build
cd build
cmake .. 
make
```
Nice and easy.
#### MacOS
Configure for XCode and compile there:
```
mkdir build
cd build
cmake .. -G "XCode"
```
You may try configuring for `make` but since OpenGL is deprecated since MacOS 10.14 Mojave you may run into troubles (just like I did).

#### Windows
You can use [vcpkg](https://github.com/microsoft/vcpkg) to install all necessary dependencies:
```
./vcpkg install GLEW:x64-windows
./vcpkg install glfw3:x64-windows
./vcpkg install glm:x64-windows
./vcpkg install stb:x64-windows
```
Then, use cmake or CMake-gui to configure the project for Visual Studio. Add variable `CMAKE_TOOLCHAIN_FILE` and set it to `path/to/vcpkg/scripts/buildsystems/vcpkg.cmake` - this will allow CMake to easily find all libraries installed with `vcpkg`. Open and build the project in Visual Studio.
