# create a target to build
add_executable(exampleScene exampleScene.cpp)

# link the project library.
# by transitivity, it provides all third-party libraries
# and necessary include directories
target_link_libraries(exampleScene PRIVATE RendGL)

