#include "BufferObjects.h"
#include "Engine/Utils/LoggingManager.h"

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

FrameBuffer::FrameBuffer() :
	m_colorAttachment(0), m_depthStencilRBO(0), m_textureTarget(0xFF)
{
	glGenFramebuffers(1, &m_ID);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_ID);
	glDeleteTextures(1, &m_colorAttachment);
	glDeleteRenderbuffers(1, &m_depthStencilRBO);
}

void FrameBuffer::attachColorBuffer(GLenum target, int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glGenTextures(1, &m_colorAttachment);
	glBindTexture(target, m_colorAttachment);

	if(target == GL_TEXTURE_2D_MULTISAMPLE)
		glTexImage2DMultisample(target, 4, GL_RGB, width, height, GL_TRUE);
	else
		glTexImage2D(target, 0, GL_RGB, 1600, 900, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_colorAttachment, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(target, 0);

	m_textureTarget = target;
}

void FrameBuffer::attachDepthStencilRBO(int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glGenRenderbuffers(1, &m_depthStencilRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilRBO);

	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void FrameBuffer::bindColorAttachment(int unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(m_textureTarget, m_colorAttachment);
}

void FrameBuffer::bindBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		OutputLog("The framebuffer trying to be bound is not complete!", Logging::Severity::FATAL);
}

void FrameBuffer::unbindBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const uint32_t& FrameBuffer::getID() const
{
	return m_ID;
}

////////////////////////////////////////////////////////////////////////////////////