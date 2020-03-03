#version 330 core
layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 normalPos;
layout (location = 2) in vec2 texturePos;

out VS_OUT
{
    vec3 fragPos;
    vec3 normalPos;
    vec2 texturePos;
} vshOut;

layout (std140) uniform Matrices
{
    mat4 view, projection;
};

uniform mat4 model;

void main()
{
    vshOut.fragPos = vec3(model * vec4(vertexPos, 1.0f));
    vshOut.normalPos = mat3(transpose(inverse(model))) * normalPos;
    vshOut.texturePos = texturePos;

    gl_Position = projection * view * model * vec4(vertexPos, 1.0f);
}
