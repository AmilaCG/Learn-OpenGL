#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

struct Light
{
    vec4 transform;
    vec3 spotDirection;
    float spotCutOff;
    float spotOuterCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light light;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};

uniform Material material;

void main()
{
    vec3 ambient = texture(material.diffuse, TexCoords).rgb * light.ambient;

    vec3 norm = normalize(Normal);

    vec3 lightDir;
    if (light.transform.w == 1.0)
    {
        // Light direction from fragment to light
        lightDir = normalize(light.transform.xyz - FragPos);
    }
    else if (light.transform.w == 0.0)
    {
        lightDir = normalize(-light.transform.xyz);
    }

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * texture(material.diffuse, TexCoords).rgb * light.diffuse;

    // The reflect function expects the first vector to point
    // from the light source towards the fragment's position. Therefore,
    // we are getting the negative value.
    vec3 reflectDir = reflect(-lightDir, norm);

    // Light reflection from fragment to camera/eye
    vec3 viewDir = normalize(viewPos - FragPos);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * texture(material.specular, TexCoords).rgb * light.specular;

    // Spotlight with soft edges
    float theta = dot(lightDir, normalize(-light.spotDirection));
    float epsilon = light.spotCutOff - light.spotOuterCutOff;
    float intensity = clamp((theta - light.spotOuterCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    // Attenuation
    float distance = length(light.transform.xyz - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                            light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}