#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
//    FragColor = vec4(vec3(gl_FragCoord.z), 1.0); // Visualize depth buffer
//    float depth = LinearizeDepth(gl_FragCoord.z) / far;
//    FragColor = vec4(vec3(depth), 1.0);

    vec4 texColor = texture(texture1, TexCoords);
    if (texColor.a < 0.1)
    {
        discard;
    }
    FragColor = texColor;
}
