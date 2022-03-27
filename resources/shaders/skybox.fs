#version 330 core
out vec4 FragColor;

in vec3 TexCoords;
uniform samplerCube daySkybox;
uniform samplerCube nightSkybox;

void main()
{
    vec4 outputColor = mix(texture(daySkybox, TexCoords), texture(nightSkybox, TexCoords), 1.0);
    FragColor = outputColor;
}