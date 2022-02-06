#include "Mesh.h"

// ==============================================================================
// =====================          MESH CLASS       ==============================
// ==============================================================================

Mesh::Mesh(const std::vector<GLfloat>& vertices,
	const std::vector<GLuint>& indices, VertexData vertexData)
{
	GLuint numVertices = vertices.size();
	m_numIndices = indices.size();

	// create a Vertex Array Object on the GPU and store its number
	glGenVertexArrays(1, &m_VAO);
	// activate VAO to add further objects to it
	glBindVertexArray(m_VAO);

	// create a Vertex Buffer Object on the GPU and store its number
	glGenBuffers(1, &m_VBO);
	// activate VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// copy vertices to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, &vertices[0], GL_STATIC_DRAW);

	if (vertexData.has(VertexData::POSITION)) {
		// map position to (location = 0) in vertex shader
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0, // (location = 0) in vertex shader
			3, // position has 3 elements
			GL_FLOAT, GL_FALSE, // float data, no need to normalize 
			sizeof(vertices[0]) * vertexData.stride(),
			(void*)(sizeof(vertices[0]) * vertexData.positionOffset()) );
	}

	if (vertexData.has(VertexData::UV)) {
		// map uv/texture coordinates to (location = 1) in vertex shader
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 
			sizeof(vertices[0]) * vertexData.stride(), 
			(void*)(sizeof(vertices[0]) * vertexData.uvOffset()) );
	}
	
	if (vertexData.has(VertexData::NORMAL)) {
		// map normal vectors coordinates to (location = 2) in vertex shader
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 
			sizeof(vertices[0]) * vertexData.stride(), 
			(void*)(sizeof(vertices[0]) * vertexData.normalOffset()) );
	}

	// create a Element Buffer Object on the GPU and store its number
	glGenBuffers(1, &m_EBO);
	// activate EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	// copy elements' indices to the GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * m_numIndices, &indices[0], GL_STATIC_DRAW);

	// Not strictly necessary but good practice 
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
	// free GPU memory
	if (m_EBO != 0)
		glDeleteBuffers(1, &m_EBO);
	if (m_VBO != 0)
		glDeleteBuffers(1, &m_VBO);
	if (m_VAO != 0)
		glDeleteVertexArrays(1, &m_VAO);
}

Mesh::Mesh(Mesh&& other) noexcept :
	m_VAO(other.m_VAO),
	m_VBO(other.m_VBO),
	m_EBO(other.m_EBO),
	m_numIndices(other.m_numIndices)
{
	other.m_VAO = 0;
	other.m_VBO = 0;
	other.m_EBO = 0;
}

void Mesh::render() const
{
	// bind VAO (also activates VBO and EBO)
	glBindVertexArray(m_VAO);
	// 0 is the offset from the beginning of the index array
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
	// deactivate VAO
	glBindVertexArray(0);
}
