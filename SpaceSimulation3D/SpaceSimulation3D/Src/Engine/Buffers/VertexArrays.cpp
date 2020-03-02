#include "VertexArrays.h"
#include <cassert>

VertexArray::VertexArray() :
	m_ID(0)
{
	glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::attachBufferObjects(std::shared_ptr<VertexBuffer> vbo, std::shared_ptr<IndexBuffer> ibo)
{
	assert(vbo != nullptr); // A vbo MUST be passed by parameter

	m_vboStack.emplace_back(vbo);
	m_iboStack.emplace_back(ibo);

	// Bind the vao, vbo and ibo (if given) then configure the vertex attrib pointers
	glBindVertexArray(m_ID);
	vbo->bindBuffer();
	if (ibo)
		ibo->bindBuffer();

	for (const auto& layout : m_layoutStack)
	{
		glEnableVertexAttribArray(layout.index);
		glVertexAttribPointer(layout.index, layout.size, layout.type, layout.normalized, layout.stride,
			layout.offset);

		glVertexAttribDivisor(layout.index, layout.divisor);
	}

	m_layoutStack.clear();

	// Finally just unbind the vao, vbo and ibo (if given)
	glBindVertexArray(0);
	vbo->unbindBuffer();
	if (ibo)
		ibo->unbindBuffer();
}

void VertexArray::bind() const
{
	glBindVertexArray(m_ID);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}

const uint32_t& VertexArray::getID() const
{
	return m_ID;
}

const std::vector<std::shared_ptr<VertexBuffer>>& VertexArray::getVBOStack() const
{
	return m_vboStack;
}

const std::vector<std::shared_ptr<IndexBuffer>>& VertexArray::getIBOStack() const
{
	return m_iboStack;
}