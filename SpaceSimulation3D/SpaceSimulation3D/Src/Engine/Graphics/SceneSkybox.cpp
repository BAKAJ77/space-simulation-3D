#include "SceneSkybox.h"
#include "Engine/Utils/LoggingManager.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <array>

Skybox::Skybox(const std::array<std::string, 6>& texture_paths) :
	m_cubemapID(0)
{
	// First setup the vbo and vao of skybox
	std::array<float, 108> vertices
	{
        // Vertex pos
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
	};

    auto vbo = std::make_shared<VertexBuffer>(vertices.data(), sizeof(vertices), GL_STATIC_DRAW);
    m_vao = std::make_shared<VertexArray>();
    m_vao->pushLayout<float>(0, 3, 3 * sizeof(float));
    m_vao->attachBufferObjects(vbo);

    // Now setup the cubemap texture object
    glGenTextures(1, &m_cubemapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (int count = 0; count < 6; count++)
    {
        int width, height, channels;
        GLubyte* textureData = stbi_load(texture_paths[count].c_str(), &width, &height, &channels, 0);
        if (textureData)
        {
            if(texture_paths[count].find(".png") != std::string::npos)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + count, 0, GL_RGBA, width, height, 0, GL_RGBA,
                    GL_UNSIGNED_BYTE, textureData);
            else
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + count, 0, GL_RGB, width, height, 0, GL_RGB,
                    GL_UNSIGNED_BYTE, textureData);
        }
        else
            OutputLog("Failed to load texture at path: " + texture_paths[count], Logging::Severity::FATAL);

        stbi_image_free(textureData);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Skybox::~Skybox()
{
    glDeleteTextures(1, &m_cubemapID);
}

void Skybox::render(std::shared_ptr<ShaderProgram> shader) const
{
    glDepthFunc(GL_LEQUAL);

    shader->bindProgram();
    shader->setUniform("skybox", 0);
    m_vao->bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapID);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}