#version 330 core

in vec3 position;
uniform samplerCube skybox;

void main()
{
    gl_FragColor = texture(skybox, position);
}
