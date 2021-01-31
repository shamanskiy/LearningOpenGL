#include "StaticModel.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

StaticModel::StaticModel(const Mesh * const mesh,
                       const Texture * const texture,
                       glm::mat4 modelMatrix) :
    m_mesh(mesh),
    m_texture(texture),
    m_modelMatrix(modelMatrix)
{}

void StaticModel::render(const Shader &shader) const
{
    glUniformMatrix4fv(shader.uniModelMatrix(),1,GL_FALSE,
                       glm::value_ptr(m_modelMatrix));
    // activate texture
    m_texture->useTexture();
    // render the floor
    m_mesh->renderMesh();
}
