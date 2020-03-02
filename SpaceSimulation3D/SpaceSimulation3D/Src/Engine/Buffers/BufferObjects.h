#pragma once
#include <glad/glad.h>

typedef unsigned int uint32_t;

class VertexBuffer
{
private:
	uint32_t m_ID;
public:
	VertexBuffer(const void* data, GLsizeiptr size, GLenum usage);
	~VertexBuffer();

	void modifyBufferData(const void* data, GLintptr offset, GLsizeiptr size);

	void bindBuffer() const; // Binds the vbo
	void unbindBuffer() const; // Unbinds the vbo
public:
	const uint32_t& getID() const; // Returns the ID of the vbo
};

class IndexBuffer
{
private:
	uint32_t m_ID;
public:
	IndexBuffer(const void* data, GLsizeiptr size, GLenum usage);
	~IndexBuffer();

	void modifyBufferData(const void* data, GLintptr offset, GLsizeiptr size);

	void bindBuffer() const; // Binds the ibo
	void unbindBuffer() const; // Unbinds the ibo
public:
	const uint32_t& getID() const; // Returns the ID of the ibo
};