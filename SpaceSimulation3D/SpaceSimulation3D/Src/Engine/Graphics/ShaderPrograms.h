#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <string>

enum class ShaderCheckType
{
	COMPILATION,
	LINKING
};

class ShaderProgram
{
private:
	uint32_t m_ID;
	const std::string m_vshPath, m_gshPath, m_fshPath;

	mutable std::unordered_map<std::string, uint32_t> m_uniformCache;
private:
	std::string loadShaderContents(const std::string& file_path) const; // Loads the shader contents into a string from file
	uint32_t queryUniformLocation(const uint32_t& id, const std::string& uniform) const; // Gets the uniform location from GPU and store in cache map
	void generateShader(uint32_t& id, const char* src, int shader_type) const; // Generates a shader object and compiles it

	void checkShaderErrors(const uint32_t& id, ShaderCheckType check_type) const; // Checks for errors in compilation or linking of shaders
public:
	ShaderProgram(const std::string& vsh_path, const std::string& fsh_path, const std::string& gsh_path = "");
	~ShaderProgram();

	void bindUniformBlock(const std::string& uniform_block, uint32_t unit) const; // Binds the uniform block in shader to binding point

	void bindProgram() const; // Binds the shader program
	void unbindProgram() const; // Unbinds the shader program
public:
	// setUniform() : Sets the value of uniform in the shader
	void setUniform(const std::string& uniform, int value) const;
	void setUniform(const std::string& uniform, float value) const;
	void setUniform(const std::string& uniform, bool value) const;

	void setUniform(const std::string& uniform, const glm::mat3& matrix) const;
	void setUniform(const std::string& uniform, const glm::mat4& matrix) const;

	void setUniform(const std::string& uniform, const glm::vec3& vector) const;
	void setUniform(const std::string& uniform, const glm::vec4& vector) const;
public:
	const uint32_t& getID() const; // Returns the ID of the shader

	const std::string& getVertexPath() const; // Returns the vertex path
	const std::string& getFragmentPath() const; // Returns the fragment path
	const std::string& getGeometryPath() const; // Returns the geometry path
};