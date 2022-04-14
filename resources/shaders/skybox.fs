#version 330 core
out vec4 FragColor;

in vec3 TexCoords;
uniform samplerCube daySkybox;
uniform samplerCube nightSkybox;
uniform float opacity;

void main()
{
    vec4 t0 = texture(daySkybox, TexCoords);
    vec4 t1 = texture(nightSkybox, TexCoords);

    vec4  outputColor = mix(t0,t1, opacity);
    vec3 color = outputColor.rgb;
    color *= vec3(0.6);
    outputColor = vec4(color, 1.0);
    FragColor = outputColor;
}