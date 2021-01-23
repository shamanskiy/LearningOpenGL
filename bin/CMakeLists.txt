# create a target to build
add_executable(exampleScene exampleScene.cpp)

# link necessary external libraries and the project library
target_link_libraries(exampleScene PRIVATE learningOpenGL ${ASSIMP_LIBRARY_DIRS}/libassimp.dylib)

# provide paths to include files
target_include_directories(exampleScene PRIVATE "${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src" )
