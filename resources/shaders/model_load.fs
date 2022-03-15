#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightCol;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;

void main()
{

    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightCol;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = diff * lightCol;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);

    float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
    vec3 specular = specularStrength * spec * lightCol;
    vec4 result = (vec4(ambient + diffuse + specular, 1.0f)) * texture(texture_diffuse1, TexCoords);
    FragColor = result;

    if( FragColor.a < 0.1)
    {
        discard;
    }
}