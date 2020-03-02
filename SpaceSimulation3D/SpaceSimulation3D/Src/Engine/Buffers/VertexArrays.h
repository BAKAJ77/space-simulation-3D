#pragma once
#include "BufferObjects.h"

#include <vector>
#include <memory>

struct VertexLayout
{
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;

	GLsizei stride;
	const void* offset;

	GLuint divisor;
};

class VertexArray
{
private:
	uint32_t m_ID;

	std::vector<std::shared_ptr<VertexBuffer>> m_vboStack;
	std::vector<std::shared_ptr<IndexBuffer>> m_iboStack;
	std::vector<VertexLayout> m_layoutStack;
public:
	VertexArray();
	~VertexArray();

	void attachBufferObjects(std::shared_ptr<VertexBuffer> vbo, 
		std::shared_ptr<IndexBuffer> ibo = nullptr); // Attaches the given vbo (and ibo) to the vao
	
	void bind() const; // Binds the vao
	void unbind() const; // Unbinds the vao
public:
	// pushLayout<>() : Pushes the vertex attrib layout to stack
	template<typename T>
	void pushLayout(GLuint index, GLint size, GLsizei stride, GLsizei offset = 0, GLuint divisor = 0, 
		GLboolean normalized = GL_FALSE);

	template<>
	void pushLayout<float>(GLuint index, GLint size, GLsizei stride, GLsizei offset, GLuint divisor, 
		GLboolean normalized);
	template<>
	void pushLayout<uint32_t>(GLuint index, GLint size, GLsizei stride, GLsizei offset, GLuint divisor,
		GLboolean normalized);
	template<>
	void pushLayout<GLubyte>(GLuint index, GLint size, GLsizei stride, GLsizei offset, GLuint divisor,
		GLboolean normalized);
public:
	const uint32_t& getID() const; // Returns the ID of the vao

	const std::vector<std::shared_ptr<VertexBuffer>>& getVBOStack() const; // Returns the stack of attached vbos
	const std::vector<std::shared_ptr<IndexBuffer>>& getIBOStack() const; // Returns the stack of attached ibos
};

#include "VertexArrays.tpp"