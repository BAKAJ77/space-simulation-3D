#pragma once
#include "Engine/Graphics/ShaderPrograms.h"
#include "Engine/Buffers/VertexArrays.h"

#include <array>
#include <string>
#include <memory>

class Skybox
{
private:
	uint32_t m_cubemapID;
	std::shared_ptr<VertexArray> m_vao;
public:
	Skybox(const std::array<std::string, 6>& texture_paths);
	~Skybox();

	void render(std::shared_ptr<ShaderProgram> shader) const;
};