#include "MeshObject.h"
#include <glad/glad.h>

MeshObject::MeshObject(const std::vector<VertexData>& vertices, const std::vector<uint32_t>& indices,
	const Material& material) :
	m_numIndices(indices.size()), m_material(material)
{
	// Setup the mesh's VBO and IBO
	auto meshVBO = std::make_shared<VertexBuffer>(&vertices[0], sizeof(VertexData) * vertices.size(), 
		GL_STATIC_DRAW);
	auto meshIBO = std::make_shared<IndexBuffer>(&indices[0], sizeof(uint32_t) * indices.size(), GL_STATIC_DRAW);

	// Finally setup the mesh's VAO
	m_vao = std::make_shared<VertexArray>();
	m_vao->pushLayout<float>(0, 3, sizeof(VertexData));
	m_vao->pushLayout<float>(1, 3, sizeof(VertexData), offsetof(VertexData, m_normalPos));
	m_vao->pushLayout<float>(2, 2, sizeof(VertexData), offsetof(VertexData, m_texturePos));
	m_vao->attachBufferObjects(meshVBO, meshIBO);
}

MeshObject::~MeshObject() {}

void MeshObject::render(std::shared_ptr<ShaderProgram> shader) const
{
	shader->setUniform("mat.shininess", m_material.shininess);

	if (m_material.m_textures.empty())
	{
		// Assign the ordinary phong component vector values in shaders
		shader->setUniform("mat.ambient", m_material.m_ambient);
		shader->setUniform("mat.diffuse", m_material.m_diffuse);
		shader->setUniform("mat.specular", m_material.m_specular);
		shader->setUniform("mat.useTextures", false);
	}
	else
	{
		// Bind the respective diffuse and specular textures
		shader->setUniform("mat.useTextures", true);
		int numDiffuse = 0, numSpecular = 0;

		for (uint32_t index = 0; index < m_material.m_textures.size(); index++)
		{
			const auto& TEXTURE_DATA = m_material.m_textures[index];

			if (TEXTURE_DATA.m_type == "DIFFUSE_TEXTURE")
				TEXTURE_DATA.m_texture->bind(shader, "mat.diffuseTexture" + std::to_string(numDiffuse++), index);
			else if (TEXTURE_DATA.m_type == "SPECULAR_TEXTURE")
				TEXTURE_DATA.m_texture->bind(shader, "mat.specularTexture" + std::to_string(numSpecular++), index);
		}
	}

	m_vao->bind();
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);
}