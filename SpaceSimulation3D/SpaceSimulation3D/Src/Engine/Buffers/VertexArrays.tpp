#include "VertexArrays.h"

template<typename T>
void VertexArray::pushLayout(GLuint index, GLint size, GLsizei stride, GLsizei offset, GLuint divisor,
	GLboolean normalized)
{
	static_assert(false);
}

template<>
void VertexArray::pushLayout<float>(GLuint index, GLint size, GLsizei stride, GLsizei offset, GLuint divisor,
	GLboolean normalized)
{
	m_layoutStack.push_back({ index, size, GL_FLOAT, normalized, stride, (void*)offset, divisor });
}

template<>
void VertexArray::pushLayout<uint32_t>(GLuint index, GLint size, GLsizei stride, GLsizei offset, GLuint divisor,
	GLboolean normalized)
{
	m_layoutStack.push_back({ index, size, GL_UNSIGNED_INT, normalized, stride, (void*)offset, divisor });
}

template<>
void VertexArray::pushLayout<GLubyte>(GLuint index, GLint size, GLsizei stride, GLsizei offset, GLuint divisor,
	GLboolean normalized)
{
	m_layoutStack.push_back({ index, size, GL_UNSIGNED_BYTE, normalized, stride, (void*)offset, divisor });
}