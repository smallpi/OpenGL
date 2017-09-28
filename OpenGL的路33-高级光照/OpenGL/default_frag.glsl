#version 330 core

//vertex shader传递过来的纹理坐标数据
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;

out vec4 oColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

uniform bool blinnPhone;
// 真正的texture像素数据
//uniform sampler2D Texture0;
//uniform sampler2D Texture1;

void main(void){
    //oColor = mix(texture(Texture0,vTexCoord),texture(Texture1,vTexCoord),0.3);

    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 normal = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vFragPos);
    float diffuseStrength = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diffuseStrength * lightColor;

    vec3 cameraDir =  normalize(cameraPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularIntensity = 0.5f;

    vec3 halfwayDir = normalize(cameraDir + lightDir);
    float specularStrength;
    if(blinnPhone){
        specularStrength = specularIntensity * pow(max(dot(halfwayDir, normal), 0.0f), 32);
    }else{
        specularStrength = specularIntensity * pow(max(dot(cameraDir, reflectDir), 0.0f), 32);
    }

    
    vec3 specular = specularStrength * lightColor;

    vec3 finalColor = (ambient + diffuse + specular) * objectColor;
    oColor = vec4(finalColor, 1.0f);
}