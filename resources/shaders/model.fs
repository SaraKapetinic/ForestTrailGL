#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct PointLight {
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPos;
uniform bool isDay;
uniform bool shadowsEnabled;
uniform int lightIndex;
uniform float ambientStrength;
uniform float far_plane;
uniform sampler2D texture_diffuse1;
uniform samplerCube depthMap;
uniform PointLight pointLights[3];

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec3 fragPos, vec3 lightPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;

    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos){
    vec3 viewDir = normalize(viewPos-light.position);
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float distance = length(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),32);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance*distance));
    float specularStrength = 0.5;

    vec3 ambient = ambientStrength * light.color ;
    vec3 diffuse = diff * light.color ;
    vec3 specular = specularStrength * spec * light.color;
    if(isDay)
        return (ambient+diffuse+specular);
    else
        return (ambient+diffuse+specular) * attenuation;
}

void main()
{
    vec4 result = vec4(0.0);
    if(!isDay){
        if(shadowsEnabled){
            result+=vec4(CalcPointLight(pointLights[lightIndex], Normal, FragPos),1.0) * texture(texture_diffuse1, TexCoords);
            float shadow = ShadowCalculation(FragPos, pointLights[lightIndex].position) - ambientStrength;
            result *= vec4( vec3(1.0 - shadow), 1.0);
            vec3 color = result.rgb;
            color = color / (color + vec3(1.0));
            result = vec4(color, result.a);
        }
        else{
            result+=vec4(CalcPointLight(pointLights[0], Normal, FragPos),1.0) * texture(texture_diffuse1, TexCoords);
            float alpha = result.a;
            result+=vec4(CalcPointLight(pointLights[1], Normal, FragPos),1.0) * texture(texture_diffuse1, TexCoords);
            vec3 color = result.rgb;
            color = color / (color + vec3(1.0));
            result = vec4(color, alpha);
        }

    }
    else {
        result+=vec4(CalcPointLight(pointLights[2], Normal, FragPos),1.0) * texture(texture_diffuse1, TexCoords);
        vec3 color = result.rgb;
        float alpha = result.a;
        color = color / (color + vec3(1.0));
        result = vec4(color, alpha);
    }

    FragColor = result;

    if( FragColor.a < 0.1)
    {
        discard;
    }

};
