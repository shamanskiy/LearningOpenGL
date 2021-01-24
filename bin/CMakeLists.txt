# create a target to build
add_executable(exampleScene exampleScene.cpp)

# link necessary external libraries and the project library
# I think that this also provides include directories
# Assimp is linked directly to exampleScene for now. Later, it will be linked through learningOpenGL
target_link_libraries(exampleScene PRIVATE learningOpenGL)

# provide paths to include files
target_include_directories(exampleScene PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/src)
