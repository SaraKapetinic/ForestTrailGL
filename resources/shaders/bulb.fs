 #version 330 core
 out vec4 FragColor;

 in vec2 TexCoords;
 in vec3 Normal;
 in vec3 FragPos;

 uniform vec3 lightCol;
 uniform vec3 viewPos;
 uniform float constant;
 uniform float linear;
 uniform float quadratic;
 uniform sampler2D texture_diffuse1;
 uniform vec3 lightPos[2];
 void main()
 {
    for(i = 0; i < 2; i++){
        float distance    = length(lightPos[i] - FragPos);
        float attenuation = 1.0 / (constant + linear * distance +
    		   quadratic * (distance * distance));
        float ambientStrength = 0.5;
        vec3 ambient = ambientStrength * lightCol*attenuation;
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos[i] - FragPos);

        float diff = max(dot(norm, lightDir), 0.0);

        vec3 diffuse = diff * lightCol * attenuation;

        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir,norm);

        float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
        vec3 specular = specularStrength * spec * lightCol * attenuation;
        vec4 result = (vec4(ambient + diffuse + specular, 1.0f)) * texture(texture_diffuse1, TexCoords);
        FragColor = result;

        if( FragColor.a < 0.1)
        {
             discard;
        }
     }
 }