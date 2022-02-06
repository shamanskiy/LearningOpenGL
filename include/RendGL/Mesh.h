#pragma once

#include <vector>

#include <GL/glew.h>

using namespace std;

class VertexData {
public:
	enum Value {
		POSITION = 1,
		UV       = 2,
		NORMAL   = 4
	};

	constexpr VertexData(Value value) : m_value(value) {}

	bool has(Value value) const { return (m_value & value) == value; }
	int stride() const { return 3*has(POSITION) + 2*has(UV) + 3*has(NORMAL); }
	int positionOffset() const { return 0; }
	int uvOffset() const { return 3*has(POSITION); }
	int normalOffset() const { return 3*has(POSITION) + 2*has(UV); }

private:
	Value m_value;
};

inline VertexData::Value operator|(VertexData::Value a, VertexData::Value b) {
	auto char_result = static_cast<char>(a) | static_cast<char>(b);
	return static_cast<VertexData::Value>(char_result);
}

inline VertexData::Value operator&(VertexData::Value a, VertexData::Value b) {
	auto char_result = static_cast<char>(a) & static_cast<char>(b);
	return static_cast<VertexData::Value>(char_result);
}

// Mesh represents a single 3D object. It gets an array of vertices
// and an array of indices, copies them to the GPU and then holds
// the pointers to the arrays of the GPU.
class Mesh
{
public:
	Mesh(const vector<GLfloat>& vertices,
		const vector<GLuint>& indices, VertexData dataTypes);
	~Mesh();
	// Copy constructor is needed for std::vector
	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& other) & noexcept = delete;

	void render() const;

private:
	// create arrays on the GPU
	void createMesh(const vector<GLfloat>& vertices,
		const vector<GLuint>& indices, VertexData dataTypes);
	// delete from GPU
	void deleteMesh();

private:
	// Vertex Array, Vertex Buffer and Element Buffer Objects
	// hold reference numbers to data objects on GPU
	GLuint m_VAO{ 0 }, m_VBO{ 0 }, m_EBO{ 0 };
	// total number of indices to draw = num of elements * 3
	GLuint m_numIndices{ 0 };
};
