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

struct SpotLight
{
    vec3 position, direction;
    vec3 ambient, diffuse, specular;

    float constant, linear, quadratic;
    float innerCutOff, outerCutOff;
    bool enabled;
};

in VS_OUT
{
    vec3 fragPos;
    vec3 normalPos;
    vec2 texturePos;
} fshIn;

uniform Material mat;
uniform DirectionalLight sunlight;
uniform SpotLight flashlight;

uniform vec3 cameraPos;

vec3 calculateDirectionalLighting(DirectionalLight light, vec3 normal);
vec3 calculateSpotLighting(SpotLight light, vec3 normal);

void main()
{
    vec3 normalDir = normalize(fshIn.normalPos);
    vec3 finalColor = calculateDirectionalLighting(sunlight, normalDir) + calculateSpotLighting(flashlight, normalDir);

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
        vec3 reflectedLightRay = reflect(-lightRay, normal);
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
        vec3 reflectedLightRay = reflect(-lightRay, normal);
        vec3 cameraDir = normalize(cameraPos - fshIn.fragPos);
        float specularStrength = pow(max(dot(reflectedLightRay, cameraDir), 0.0f), mat.shininess);

        specular = mat.specular * specularStrength * light.specular;
    }

    return (ambient + diffuse + specular);
}

vec3 calculateSpotLighting(SpotLight light, vec3 normal)
{
    vec3 ambient, diffuse, specular;
    if(light.enabled)
    {
        if(mat.useTextures)
        {
            // Ambient calculations
            ambient = texture(mat.diffuseTexture0, fshIn.texturePos).rgb * light.ambient;

            // Diffuse calculations
            vec3 lightRay = normalize(-light.direction);
            float diffuseStrength = max(dot(lightRay, normal), 0.0f);

            diffuse = texture(mat.diffuseTexture0, fshIn.texturePos).rgb * diffuseStrength * light.diffuse;

            // Specular calculations
            vec3 reflectedLightRay = reflect(-lightRay, normal);
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
            vec3 reflectedLightRay = reflect(-lightRay, normal);
            vec3 cameraDir = normalize(cameraPos - fshIn.fragPos);
            float specularStrength = pow(max(dot(reflectedLightRay, cameraDir), 0.0f), mat.shininess);

            specular = mat.specular * specularStrength * light.specular;
        }

        // Calculate the attenuation value
        float distanceVal = length(light.position - fshIn.fragPos);
        float attenuation = 1.0f / (light.constant + distanceVal * light.linear + (distanceVal * distanceVal) * light.quadratic);

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        // Interpolate the boundaries of the lit area in the direction of the light
        vec3 lightRay = normalize(light.position - fshIn.fragPos);

        float theta = dot(lightRay, normalize(-light.direction));
        float epsilon = light.innerCutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

        diffuse *= intensity;
        specular *= intensity;
    }

    return (ambient + diffuse + specular);
}
