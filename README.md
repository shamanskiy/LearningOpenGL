# LearningOpenGL
LearningOpenGL is a small project based on [a great OpenGL course by Ben Cook on Udemy.com.](https://www.udemy.com/course/graphics-with-modern-opengl)

The project follows guidelines of modern OpenGL and demonstrates the complete rendering pipeline, object importing, shading, interactive camera and more.

## Dependencies
The following libraries have to be installed to compile the project:
* OpenGL (you probably have it)
* GLEW - for [managing OpenGL extensions](http://glew.sourceforge.net/)
* glfw3 - for [window and input management](https://www.glfw.org/)
* glm - for [linear algebra](https://glm.g-truc.net/0.9.9/index.html) (header-only)
* stb_image - for [texture loading](https://github.com/nothings/stb/blob/master/stb_image.h) (header-only)

## How to build

#### Ubuntu
Nice and easy:
```
mkdir build
cd build
cmake .. 
make
```
#### MacOS
Configure for XCode and compile there:
```
mkdir build
cd build
cmake .. -G "XCode"
```
You may try configuring for `make` but since OpenGL is deprecated since MacOS 10.14 Mojave you may run into troubles (just like I did).

#### Windows
I haven't tested it yet, sorry. But you may try. Good luck!
