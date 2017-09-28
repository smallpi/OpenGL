#version 330 core

//vertex shader传递过来的纹理坐标数据
in vec2 gTexCoord;
in vec3 gNormal;
in vec3 gFragPos;
in mat3 gTBN;

in vec3 gTangentLightPos;
in vec3 gTangentCameraPos;
in vec3 gTangentFragPos;

out vec4 oColor;

// 真正的texture像素数据
uniform sampler2D Texture;
uniform sampler2D normalTexture;
uniform sampler2D heightTexture;
// 光照
uniform vec3 lightPos;
uniform vec3 cameraPos;


vec2 parallaxMapping(vec2 texCoord, vec3 viewDir){

    // Steep Parallax Mapping
    // const float numLayers = 10;
    // float layerDepth = 1.0 / numLayers;
    // float currentLayerDepth = 0.0;
    // vec2 offset = viewDir.xy * 0.03 / numLayers;

    // vec2 currentTexCoord = texCoord;
    // float currentDepthValue = texture(heightTexture, currentTexCoord).x;
    // while(currentLayerDepth < currentDepthValue){
    //     currentTexCoord -= offset;
    //     currentDepthValue = texture(heightTexture, currentTexCoord).x;
    //     currentLayerDepth += layerDepth;
    // }
    // return currentTexCoord;


    float height = texture(heightTexture, texCoord).x; // 采样高度
    vec2 offset = viewDir.xy * height * 0.03;   // 0.03需要进行调的参数
    return (texCoord - offset);
}


void main(void){

    vec3 normal = vec3(texture(normalTexture,gTexCoord));
    normal = normal * 2.0 - 1.0;
    //normal = normalize(gTBN * normal);

    vec3 lightColor = vec3(1.0f);
    // vec3 lightDir = normalize(lightPos - gFragPos);
    // vec3 viewDir = normalize(cameraPos - gFragPos);
    // vec3 reflectDir = reflect(-lightDir, normal);
    // vec3 halfwayDir = normalize(reflectDir + viewDir);
    // TBN转后的空间
    vec3 lightDir = normalize(gTangentLightPos - gTangentFragPos);
    vec3 viewDir = normalize(gTangentCameraPos - gTangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(reflectDir + viewDir);

    //  增加Parallax Mapping
    vec2 offsetTexCoord = parallaxMapping(gTexCoord, viewDir);
    // if(offsetTexCoord.x > 1.0 || 
    //     offsetTexCoord.y > 1.0 || 
    //     offsetTexCoord.x < 0.0 || 
    //     offsetTexCoord.y < 0.0)
    // {
    //     discard;
    // }

    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    float diffuseStrength;
    diffuseStrength = max(dot(lightDir, normal),0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    float specularStrength;
    specularStrength = pow(max(dot(halfwayDir, normal),0.0), 32);
    vec3 specular = specularStrength * lightColor;

    // vec3 finalColor = (ambient + diffuse + specular) * texture(Texture,gTexCoord);
    vec3 finalColor = (ambient + diffuse + specular) * texture(Texture,offsetTexCoord);

    oColor = vec4(finalColor, 1.0f);
    //oColor = texture(Texture, gTexCoord);
}