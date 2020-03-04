#version 330 core

in vec2 textureCoord;
uniform sampler2DMS colorAttachment0;
uniform int numSamples;

void main()
{
    // Sample the color value stored by each sample in the fragment
    vec4 sampledColor = vec4(0.0f);
    for(int index = 0; index < numSamples; index++)
        sampledColor += texelFetch(colorAttachment0, ivec2(gl_FragCoord), index);

    gl_FragColor = (sampledColor / numSamples); // Get the average color value
}
