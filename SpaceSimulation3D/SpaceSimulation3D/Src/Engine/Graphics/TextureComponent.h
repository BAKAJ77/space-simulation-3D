#pragma once
#include "Engine/Graphics/ShaderPrograms.h"
#include <memory>

class TextureComponent
{
private:
	uint32_t m_ID;
	int m_width, m_height, m_channels;
public:
	TextureComponent(const std::string& path, bool flip_on_load = false);
	~TextureComponent();

	void SetFilter(uint32_t min, uint32_t mag) const; // Sets the filtering algorithm used on the texture
	void SetWrap(uint32_t s_axis, uint32_t t_axis) const; // Sets the wrapping method used on the texture

	void bind(std::shared_ptr<ShaderProgram> shader, const std::string& sampler, int unit) const; // Binds the texture
	void unbind() const; // Unbinds the texture
public:
	const uint32_t& getID() const; // Returns the ID of the texture

	const int& getWidth() const; // Returns width of texture
	const int& getHeight() const; // Returns height of texture
	const int& getChannels() const; // Returns the number of channels in texture
};