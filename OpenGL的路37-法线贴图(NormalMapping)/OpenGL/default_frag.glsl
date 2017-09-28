#version 330 core

//vertex shader传递过来的纹理坐标数据
in vec2 gTexCoord;
in vec3 gNormal;
in vec3 gFragPos;
in mat3 gTBN;


out vec4 oColor;

// 真正的texture像素数据
uniform sampler2D Texture;
uniform sampler2D normalTexture;
// 光照
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main(void){

    vec3 normal = vec3(texture(normalTexture,gTexCoord));
    normal = normal * 2.0 - 1.0;
    normal = normalize(gTBN * normal);

    vec3 lightColor = vec3(1.0f);
    vec3 lightDir = normalize(lightPos - gFragPos);
    vec3 viewDir = normalize(cameraPos - gFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(reflectDir + viewDir);

    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    float diffuseStrength;
    diffuseStrength = max(dot(lightDir, normal),0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    float specularStrength;
    specularStrength = pow(max(dot(halfwayDir, normal),0.0), 32);
    vec3 specular = specularStrength * lightColor;

    vec3 finalColor = (ambient + diffuse + specular) * texture(Texture,gTexCoord);

    oColor = vec4(finalColor, 1.0f);
    //oColor = texture(Texture, gTexCoord);
}