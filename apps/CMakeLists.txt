# create targets to build
add_executable(exampleScene exampleScene.cpp)
add_executable(firstWindow firstWindow.cpp)

# link the project library to the targets
# by transitivity, it provides all third-party libraries
# and necessary include directories
target_link_libraries(exampleScene PRIVATE RendGL)
target_link_libraries(firstWindow PRIVATE RendGL)

