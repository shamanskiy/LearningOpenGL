# create targets to build
add_executable(exampleSceneOld exampleSceneOld.cpp)
add_executable(exampleScene exampleScene.cpp)

# link the project library to the targets
# by transitivity, it provides all third-party libraries
# and necessary include directories
target_link_libraries(exampleSceneOld PRIVATE RendGL)
target_link_libraries(exampleScene PRIVATE RendGL)
