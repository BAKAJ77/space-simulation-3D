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

void VertexBuffer::modifyData(const void* data, GLintptr offset, GLsizeiptr size) const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void IndexBuffer::modifyData(const void* data, GLintptr offset, GLsizeiptr size) const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

UniformBuffer::UniformBuffer(const void* data, GLsizeiptr size, GLenum usage)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
	glBufferData(GL_UNIFORM_BUFFER, size, data, usage);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void UniformBuffer::setBindingPoint(uint32_t unit, GLintptr offset, GLsizeiptr size) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
	glBindBufferRange(GL_UNIFORM_BUFFER, unit, m_ID, offset, size);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::modifyData(const void* data, GLintptr offset, GLsizeiptr size) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::bindBuffer() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
}

void UniformBuffer::unbindBuffer() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

const uint32_t& UniformBuffer::getID() const
{
	return m_ID;
}

////////////////////////////////////////////////////////////////////////////////////