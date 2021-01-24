#pragma once

#include <glm/glm.hpp>

class Mesh;
class Texture;
class Shader;

// Static model represents an object in the world which doesn't change with time.
// It is defined by a mesh, a texture and a model/transformation matrix.
// Several static models can use the same mesh or texture.
class StaticModel
{
public:
    StaticModel(const Mesh * const mesh,
               const Texture * const texture,
               glm::mat4 modelMatrix);
    
    void render(const Shader & shader) const;
    
private:
    // non-owning pointers
    const Mesh * const m_mesh;
    const Texture * const m_texture;
    glm::mat4 m_modelMatrix;
};
