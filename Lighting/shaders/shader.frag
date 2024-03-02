#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

void main()
{
    vec3 ambient = material.ambient * light.ambient;

    vec3 norm = normalize(Normal);
    // Light direction from fragment to light
    vec3 lightDir = normalize(light.position - FragPos);

    // The reflect function expects the first vector to point
    // from the light source towards the fragment's position. Therefore,
    // we are getting the negative value.
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    // Light reflection from fragment to camera/eye
    vec3 viewDir = normalize(viewPos - FragPos);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (spec * material.specular) * light.specular;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}