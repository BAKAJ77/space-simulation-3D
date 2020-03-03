#version 330 core
layout (location = 0) in vec3 vertexPos;

layout (std140) uniform Matrices
{
    mat4 view, projection;
};

out vec3 position;

void main()
{
    position = vertexPos;
    gl_Position = (projection * mat4(mat3(view)) * vec4(vertexPos, 1.0f)).xyww;
}
