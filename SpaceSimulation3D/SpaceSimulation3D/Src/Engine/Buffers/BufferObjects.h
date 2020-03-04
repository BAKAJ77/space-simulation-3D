#pragma once
#include <glad/glad.h>
#include <vector>

typedef unsigned int uint32_t;

class VertexBuffer
{
private:
	uint32_t m_ID;
public:
	VertexBuffer(const void* data, GLsizeiptr size, GLenum usage);
	~VertexBuffer();

	void modifyData(const void* data, GLintptr offset, GLsizeiptr size) const; // Modifys the data in the buffer memory

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

	void modifyData(const void* data, GLintptr offset, GLsizeiptr size) const; // Modifys the data in the buffer memory

	void bindBuffer() const; // Binds the ibo
	void unbindBuffer() const; // Unbinds the ibo
public:
	const uint32_t& getID() const; // Returns the ID of the ibo
};

class UniformBuffer
{
private:
	uint32_t m_ID;
public:
	UniformBuffer(const void* data, GLsizeiptr size, GLenum usage);
	~UniformBuffer();

	void setBindingPoint(uint32_t unit, GLintptr offset, GLsizeiptr size) const; // Sets the uniform binding point of the buffer
	void modifyData(const void* data, GLintptr offset, GLsizeiptr size) const; // Modifys the data in the buffer memory

	void bindBuffer() const; // Binds the ubo
	void unbindBuffer() const; // Unbinds the ubo
public:
	const uint32_t& getID() const; // Returns the ID of the ubo
};

class FrameBuffer
{
private:
	uint32_t m_ID, m_colorAttachment, m_depthStencilRBO;
	GLenum m_textureTarget;
public:
	FrameBuffer();
	~FrameBuffer();

	void attachColorBuffer(GLenum target, int width, int height); // Attaches a color buffer to the fbo
	void attachDepthStencilRBO(int width, int height); // Attaches a depth and stencil rbo to the fbo

	void bindColorAttachment(int unit) const; // Binds the color buffer attached to the fbo
	void bindBuffer() const; // Binds the fbo

	void unbindBuffer() const; // Unbinds the fbo
public:
	const uint32_t& getID() const; // Returns the ID of the fbo
};