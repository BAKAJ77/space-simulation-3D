#include "ShaderPrograms.h"
#include "Engine/Utils/LoggingManager.h"

#include <glad/glad.h>
#include <sstream>
#include <fstream>

ShaderProgram::ShaderProgram(const std::string& vsh_path, const std::string& fsh_path, const std::string& gsh_path) :
	m_vshPath(vsh_path), m_fshPath(fsh_path), m_gshPath(gsh_path)
{
	// Load the contents of the shader files
	std::string vshStr = this->loadShaderContents(vsh_path);
	std::string fshStr = this->loadShaderContents(fsh_path);
	std::string gshStr = this->loadShaderContents(gsh_path);

	// Next we generate shader objects and compile them
	uint32_t vshID, fshID, gshID = 0;

	const char* vshSrc = vshStr.c_str();
	this->generateShader(vshID, vshSrc, GL_VERTEX_SHADER);
	const char* fshSrc = fshStr.c_str();
	this->generateShader(fshID, fshSrc, GL_FRAGMENT_SHADER);

	if (gsh_path != "")
	{
		const char* gshSrc = gshStr.c_str();
		this->generateShader(gshID, gshSrc, GL_GEOMETRY_SHADER);
	}

	// Attach the shader objects to the shader program, then link them
	m_ID = glCreateProgram();

	glAttachShader(m_ID, vshID);
	glAttachShader(m_ID, fshID);
	if(gsh_path != "")
		glAttachShader(m_ID, gshID);

	glLinkProgram(m_ID);
	this->checkShaderErrors(m_ID, ShaderCheckType::LINKING);

	// Finally free the shader objects, we don't need them anymore
	glDeleteShader(vshID);
	glDeleteShader(fshID);
	if(gsh_path != "")
		glDeleteShader(vshID);
}

std::string ShaderProgram::loadShaderContents(const std::string& file_path) const
{
	if (file_path != "")
	{
		// Open the shader file to be loaded
		std::ifstream shaderFile(file_path);
		if (shaderFile.fail())
			OutputLog("A problem occurred while loading the contents of shader at path: " + file_path,
				Logging::Severity::FATAL);

		// Load the contents of the file
		std::stringstream contentStream;
		contentStream << shaderFile.rdbuf();
		shaderFile.close();

		return contentStream.str();
	}
	else
		return "";
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_ID);
}

void ShaderProgram::generateShader(uint32_t& id, const char* src, int shader_type) const
{
	id = glCreateShader(shader_type);
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	this->checkShaderErrors(id, ShaderCheckType::COMPILATION);
}

void ShaderProgram::checkShaderErrors(const uint32_t& id, ShaderCheckType check_type) const
{
	int logLength = 0;
	switch (check_type)
	{
	case ShaderCheckType::COMPILATION:
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			auto errorLog = std::unique_ptr<char>(new char[logLength]);
			glGetShaderInfoLog(id, logLength, nullptr, errorLog.get());
			OutputLog(errorLog.get(), Logging::Severity::FATAL);
		}
		break;
	case ShaderCheckType::LINKING:
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			auto errorLog = std::unique_ptr<char>(new char[logLength]);
			glGetProgramInfoLog(id, logLength, nullptr, errorLog.get());
			OutputLog(errorLog.get(), Logging::Severity::FATAL);
		}
		break;
	}
}

uint32_t ShaderProgram::queryUniformLocation(const uint32_t& id, const std::string& uniform) const
{
	if (m_uniformCache.find(uniform) == m_uniformCache.end())
	{
		const uint32_t LOCATION = glGetUniformLocation(id, uniform.c_str());
		m_uniformCache[uniform] = LOCATION;
	}

	return m_uniformCache[uniform];
}

void ShaderProgram::bindProgram() const
{
	glUseProgram(m_ID);
}

void ShaderProgram::unbindProgram() const
{
	glUseProgram(0);
}

void ShaderProgram::setUniform(const std::string& uniform, int value) const
{
	glUniform1i(this->queryUniformLocation(m_ID, uniform.c_str()), value);
}

void ShaderProgram::setUniform(const std::string& uniform, float value) const
{
	glUniform1f(this->queryUniformLocation(m_ID, uniform.c_str()), value);
}

void ShaderProgram::setUniform(const std::string& uniform, bool value) const
{
	glUniform1i(this->queryUniformLocation(m_ID, uniform.c_str()), static_cast<int>(value));
}

void ShaderProgram::setUniform(const std::string& uniform, const glm::mat3& matrix) const
{
	glUniformMatrix3fv(this->queryUniformLocation(m_ID, uniform.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void ShaderProgram::setUniform(const std::string& uniform, const glm::mat4& matrix) const
{
	glUniformMatrix4fv(this->queryUniformLocation(m_ID, uniform.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void ShaderProgram::setUniform(const std::string& uniform, const glm::vec3& vector) const
{
	glUniform3fv(this->queryUniformLocation(m_ID, uniform.c_str()), 1, &vector[0]);
}

void ShaderProgram::setUniform(const std::string& uniform, const glm::vec4& vector) const
{
	glUniform4fv(this->queryUniformLocation(m_ID, uniform.c_str()), 1, &vector[0]);
}

const uint32_t& ShaderProgram::getID() const
{
	return m_ID;
}

const std::string& ShaderProgram::getVertexPath() const
{
	return m_vshPath;
}

const std::string& ShaderProgram::getFragmentPath() const
{
	return m_fshPath;
}

const std::string& ShaderProgram::getGeometryPath() const
{
	return m_gshPath;
}