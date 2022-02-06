# RendGL provides all necessary libraries, include directories and settings
add_executable(exampleScene exampleScene.cpp)
target_link_libraries(exampleScene PRIVATE RendGL)

add_executable(firstWindow firstWindow.cpp)
target_link_libraries(firstWindow PUBLIC GLEW::GLEW glfw)

add_executable(firstTriangle firstTriangle.cpp)
target_link_libraries(firstTriangle PRIVATE RendGL)

add_executable(rotatingSquare rotatingSquare.cpp)
target_link_libraries(rotatingSquare PRIVATE RendGL)