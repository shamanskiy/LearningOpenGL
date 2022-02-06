#include "Mesh.h"

// ==============================================================================
// =====================          MESH CLASS       ==============================
// ==============================================================================

Mesh::Mesh(const std::vector<GLfloat>& vertices,
	const std::vector<GLuint>& indices, VertexData dataTypes)
{
	createMesh(vertices, indices, dataTypes);
}

Mesh::~Mesh()
{
	deleteMesh();
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


void Mesh::createMesh(const vector<GLfloat>& vertices,
	const vector<GLuint>& indices, VertexData vertexData)
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

	// map position to vertex shader
	if (vertexData.has(VertexData::POSITION)) {
		// enable (location = 0) in vertex shader, disable by default
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0, // (location = 0) in vertex shader
			3, // position has 3 elements
			GL_FLOAT, GL_FALSE, // float data, no need to normalize 
			sizeof(vertices[0]) * vertexData.stride(),
			(void*)(sizeof(vertices[0]) * vertexData.positionOffset()) );
	}

	// map uv/texture coordinates to vertex shader
	if (vertexData.has(VertexData::UV)) {
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 
			sizeof(vertices[0]) * vertexData.stride(), 
			(void*)(sizeof(vertices[0]) * vertexData.uvOffset()) );
	}
	
	// map normal vectors to vertex shader
	if (vertexData.has(VertexData::NORMAL)) {
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

	// deactivate VAO, no need to edit this object (should it be here or after VBO and EBO?)
	glBindVertexArray(0);
	// deactivate VBO, no need to edit the vertex data any more
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// deactivate EBO, no need to edit the index data any more
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

void Mesh::deleteMesh()
{
	// free GPU memory
	if (m_EBO != 0)
		glDeleteBuffers(1, &m_EBO);
	if (m_VBO != 0)
		glDeleteBuffers(1, &m_VBO);
	if (m_VAO != 0)
		glDeleteVertexArrays(1, &m_VAO);
}

