#version 330 core
layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 texturePos;

out vec2 textureCoord;

void main()
{
    textureCoord = texturePos;
    gl_Position = vec4(vertexPos * 2.0f, 0.0f, 1.0f);
}
