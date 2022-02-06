#pragma once

#include <vector>

#include <GL/glew.h>

using namespace std;

// Use as a bit mask to specify data types in a vertex array.
// For example, if the data contain positions and normals,
// pass VertexData::POSITION | VertexData::NORMAL to Mesh. 
class VertexData {
public:
	enum Value {
		POSITION = 1,
		UV       = 2,
		NORMAL   = 4
	};

	constexpr VertexData(Value value) : m_value(value) {}

	constexpr bool has(Value value) const { return (m_value & value) == value; }
	constexpr int stride() const { return 3*has(POSITION) + 2*has(UV) + 3*has(NORMAL); }
	constexpr int positionOffset() const { return 0; }
	constexpr int uvOffset() const { return 3*has(POSITION); }
	constexpr int normalOffset() const { return 3*has(POSITION) + 2*has(UV); }

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

// Mesh represents a single 3D object. It holds pointers to vertex data on the GPU.
// The format of vertex data is POSITION -> UV (optional) -> NORMALs (optional).
class Mesh
{
public:
	Mesh(const vector<GLfloat>& vertices,
		const vector<GLuint>& indices, VertexData vertexData);
	~Mesh();
	// Copy constructor is needed for std::vector
	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& other) & noexcept = delete;

	void render() const;

private:
	// Vertex Array, Vertex Buffer and Element Buffer Objects
	// hold reference numbers to data objects on GPU
	GLuint m_VAO{ 0 }, m_VBO{ 0 }, m_EBO{ 0 };
	// total number of indices to draw = num of elements * 3
	GLuint m_numIndices{ 0 };
};
