#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightCol;
uniform vec3 lightPos;
uniform sampler2D texture_diffuse1;

void main()
{
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightCol;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = diff * lightCol;
    vec4 result = (vec4(ambient + diffuse, 1.0f)) * texture(texture_diffuse1, TexCoords);
    FragColor = result;
}