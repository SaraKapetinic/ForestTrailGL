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
uniform int lightIndex;
uniform bool shadowsEnabled;
uniform sampler2D waterText;
uniform PointLight pointLights[3];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos){
    vec3 viewDir = normalize(viewPos-light.position);
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float distance = length(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),32);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance*distance));

    float ambientStrength = 8.0;
    float specularStrength = 0.5;

    vec3 ambient = ambientStrength * light.color * attenuation;
    vec3 diffuse = diff * light.color * attenuation;
    vec3 specular = specularStrength * spec * light.color * attenuation;

    return (ambient+diffuse+specular);
}

void main()
{
    vec4 result = vec4(0.0);
    vec4 texColor = texture(waterText, TexCoords / 25.0);
    if(!isDay){
        if(shadowsEnabled){
            result+=vec4(CalcPointLight(pointLights[lightIndex], Normal, FragPos),1.0) * texColor;
            result[3] = 0.3;
        }
        else {
            result+=vec4(CalcPointLight(pointLights[0], Normal, FragPos),1.0) * texColor;
            result+=vec4(CalcPointLight(pointLights[1], Normal, FragPos),1.0) * texColor;
            result*=vec4(0.2);
            result[3] = 0.3;
        }

    }
    else {
        result+=  texColor;
        result.a = 0.3;

    }
    vec3 color = result.rgb;
    result = vec4(color, 0.2);
    FragColor = result;
    if( FragColor.a < 0.1)
    {
        discard;
    }
};
