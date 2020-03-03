#include "TextureComponent.h"
#include "Engine/Utils/LoggingManager.h"

#include <glad/glad.h>
#include <stb_image.h>

TextureComponent::TextureComponent(const std::string& path, bool flip_on_load)
{
	// Generate the texture and configure its filtering and wrapping methods
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Load the texture data from file and into the allocated texture buffer
	stbi_set_flip_vertically_on_load(flip_on_load);
	GLubyte* textureData = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
	if (textureData)
	{
		if (path.find(".png") != std::string::npos)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		OutputLog("An error occurred while loading the texture at path: " + path, Logging::Severity::FATAL);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(textureData);
}

TextureComponent::~TextureComponent()
{
	glDeleteTextures(1, &m_ID);
}

void TextureComponent::SetFilter(uint32_t min, uint32_t mag) const
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureComponent::SetWrap(uint32_t s_axis, uint32_t t_axis) const
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_axis);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_axis);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureComponent::bind(std::shared_ptr<ShaderProgram> shader, const std::string& sampler, int unit) const
{
	shader->setUniform(sampler, unit);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void TextureComponent::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

const uint32_t& TextureComponent::getID() const
{
	return m_ID;
}

const int& TextureComponent::getWidth() const
{
	return m_width;
}

const int& TextureComponent::getHeight() const
{
	return m_height;
}

const int& TextureComponent::getChannels() const
{
	return m_channels;
}