#pragma once
#include "Engine/Buffers/VertexArrays.h"
#include "Engine/Graphics/TextureComponent.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>

struct VertexData
{
	glm::vec3 m_vertexPos;
	glm::vec3 m_normalPos;
	glm::vec2 m_texturePos;
};

struct TextureData
{
	std::string m_type;
	std::shared_ptr<TextureComponent> m_texture;
};

struct Material
{
	std::vector<TextureData> m_textures;
	glm::vec3 m_ambient, m_diffuse, m_specular;
	float shininess;
};

class MeshObject
{
private:
	std::shared_ptr<VertexArray> m_vao;
	uint32_t m_numIndices, m_numInstances;
	bool m_instanced;
	
	Material m_material;
public:
	MeshObject(const std::vector<VertexData>& vertices, const std::vector<uint32_t>& indices,
		const Material& material, const void* instances_array = nullptr, uint32_t num_instances = 0);
	~MeshObject();

	void render(std::shared_ptr<ShaderProgram> shader) const;
};