#include "BufferObjects.h"

////////////////////////////////////////////////////////////////////////////////////

VertexBuffer::VertexBuffer(const void* data, GLsizeiptr size, GLenum usage)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void VertexBuffer::modifyBufferData(const void* data, GLintptr offset, GLsizeiptr size)
{
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::bindBuffer() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VertexBuffer::unbindBuffer() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const uint32_t& VertexBuffer::getID() const
{
	return m_ID;
}

////////////////////////////////////////////////////////////////////////////////////

IndexBuffer::IndexBuffer(const void* data, GLsizeiptr size, GLenum usage)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void IndexBuffer::modifyBufferData(const void* data, GLintptr offset, GLsizeiptr size)
{
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

void IndexBuffer::bindBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::unbindBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

const uint32_t& IndexBuffer::getID() const
{
	return m_ID;
}

////////////////////////////////////////////////////////////////////////////////////