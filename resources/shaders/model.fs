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
uniform float ambientStrength;
uniform sampler2D texture_diffuse1;
uniform PointLight pointLights[3];



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
        result+=vec4(CalcPointLight(pointLights[0], Normal, FragPos),1.0) * texture(texture_diffuse1, TexCoords);
        result+=vec4(CalcPointLight(pointLights[1], Normal, FragPos),1.0) * texture(texture_diffuse1, TexCoords);
    }
    else {
        result+=vec4(CalcPointLight(pointLights[2], Normal, FragPos),1.0) * texture(texture_diffuse1, TexCoords);
    }
    FragColor = result;

    if( FragColor.a < 0.1)
    {
        discard;
    }

};
