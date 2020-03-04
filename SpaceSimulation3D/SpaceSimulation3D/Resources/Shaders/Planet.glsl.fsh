#version 330 core

struct Material
{
    sampler2D diffuseTexture0, specularTexture0;
    vec3 ambient, diffuse, specular;
    float shininess;

    bool useTextures;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient, diffuse, specular;
};

in VS_OUT
{
    vec3 fragPos;
    vec3 normalPos;
    vec2 texturePos;
} fshIn;

uniform Material mat;
uniform DirectionalLight sunlight;
uniform vec3 cameraPos;

vec3 calculateDirectionalLighting(DirectionalLight light, vec3 normal);

void main()
{
    vec3 normalDir = normalize(fshIn.normalPos);
    vec3 finalColor = calculateDirectionalLighting(sunlight, normalDir);

    gl_FragColor = vec4(finalColor, 1.0f);
}

vec3 calculateDirectionalLighting(DirectionalLight light, vec3 normal)
{
    vec3 ambient, diffuse, specular;
    if(mat.useTextures)
    {
        // Ambient calculations
        ambient = texture(mat.diffuseTexture0, fshIn.texturePos).rgb * light.ambient;

        // Diffuse calculations
        vec3 lightRay = normalize(-light.direction);
        float diffuseStrength = max(dot(lightRay, normal), 0.0f);

        diffuse = texture(mat.diffuseTexture0, fshIn.texturePos).rgb * diffuseStrength * light.diffuse;

        // Specular calculations
        vec3 reflectedLightRay = reflect(-light.direction, normal);
        vec3 cameraDir = normalize(cameraPos - fshIn.fragPos);
        float specularStrength = pow(max(dot(reflectedLightRay, cameraDir), 0.0f), mat.shininess);

        specular = texture(mat.specularTexture0, fshIn.texturePos).rgb * specularStrength * light.specular;
    }
    else
    {
        // Ambient calculations
        ambient = mat.ambient * light.ambient;

        // Diffuse calculations
        vec3 lightRay = normalize(-light.direction);
        float diffuseStrength = max(dot(lightRay, normal), 0.0f);

        diffuse = mat.diffuse * diffuseStrength * light.diffuse;

        // Specular calculations
        vec3 reflectedLightRay = reflect(-light.direction, normal);
        vec3 cameraDir = normalize(cameraPos - fshIn.fragPos);
        float specularStrength = pow(max(dot(reflectedLightRay, cameraDir), 0.0f), mat.shininess);

        specular = mat.specular * specularStrength * light.specular;
    }

    return (ambient + diffuse + specular);
}
