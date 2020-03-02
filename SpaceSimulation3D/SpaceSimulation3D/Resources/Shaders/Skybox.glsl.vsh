#version 330 core
layout (location = 0) in vec3 vertex_pos;

out vec3 position;
uniform mat4 view, projection;

void main()
{
    position = vertex_pos;
    gl_Position = (projection * mat4(mat3(view)) * vec4(vertex_pos, 1.0f)).xyww;
}
