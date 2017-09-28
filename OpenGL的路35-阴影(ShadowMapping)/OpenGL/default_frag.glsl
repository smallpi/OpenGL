#version 330 core

// 材质结构体
// struct Material{
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
//     float shininess;
// };

// 使用texture作为材质diffuse和ambient的颜色
// 使用texture作为材质specular的颜色
struct Material{
    sampler2D diffuse; // ambient和diffuse用同一种颜色
    sampler2D specular; 
    float shininess;
};


// 平行光源 Directional Light
struct DirLight{
    vec3 dir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// 点光源 Point Light
struct PointLight{
    vec3 pos;
    float kc;
    float kl;
    float kq;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// 聚光灯源 Spot Light
struct SpotLight{
    vec3 pos;
    vec3 dir;
    float innerCutOff;
    float outerCutoff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


//vertex shader传递过来的纹理坐标数据
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;
in vec4 vFragPosDepthmap;

out vec4 oColor;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform vec3 cameraPos;
// 真正的texture像素数据
//uniform sampler2D Texture0;
//uniform sampler2D Texture1;

uniform sampler2D depthmapTexture;


vec3 calcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 cameraPos);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 cameraPos);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 cameraPos);



float toLinearDepth(float depth)
{
    float near_plane = 0.1f;
    float far_plane = 100.0f;
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}


float calcShadow(vec4 fragPosDepthmap, vec3 normal, vec3 lightDir){
    // clip space 转换到normalized device coordinate
    vec3 fragPos = fragPosDepthmap.xyz / fragPosDepthmap.w;  
    // 转换到[0,1]
    fragPos = fragPos * 0.5f + 0.5f;

    // 解决由于采样引起的深度误差问题
    float bias = max(0.01 * (1.0 - dot(normalize(normal), normalize(lightDir)) ), 0.001);
    float currentDepth = fragPos.z;
    // 点光源阴影时需要将depth都转换成线性
    // float currentDepth = toLinearDepth(fragPos.z);
    

    // 超出光线远平面的默认认为不在阴影下
    // if(currentDepth > 1.0f){
    //     return 0.0f;
    // }

    // 解决depthmap分辨率引起的锯齿问题
    // textureSize可以材质图的大小，第二个参数时LOD level
    float shadow = 0.0f;
    vec2 texelSize = 1.0 / textureSize(depthmapTexture, 0);
    for(int x=-1; x<=1; x++){
        for(int y=-1; y<=1; y++){
            float closestDepth = texture(depthmapTexture, vec2(x,y) * texelSize + fragPos.xy).r;
            // 点光源阴影时需要将depth都转换成线性
            // closestDepth = LinearizeDepth(closestDepth);
            shadow += currentDepth - bias > closestDepth ? 1.0f : 0.0f;
        }
    }
    return shadow / 9.0f;
}

void main(void){

    float shadow = calcShadow(vFragPosDepthmap, vNormal, dirLight.dir);

    vec3 finalColor = calcDirLight(dirLight, vNormal, vFragPos, cameraPos);
    finalColor += calcPointLight(pointLight, vNormal, vFragPos, cameraPos);
    finalColor = (1.0f - shadow) * finalColor;

    oColor = vec4(finalColor, 1.0f);
}

// 计算平行光产生的颜色
vec3 calcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 cameraPos){
    vec3 ambientIntensity = vec3(0.3f); 
    vec3 ambient = ambientIntensity * light.ambient * vec3(texture(material.diffuse, vTexCoord));

    vec3 lightDir = normalize(light.dir);
    vec3 diffuseIntensity = vec3(1.0f);
    float diffuseStrength = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diffuseIntensity * diffuseStrength * light.diffuse * vec3(texture(material.diffuse, vTexCoord));

    vec3 cameraDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specularIntensity = vec3(1.0f);
    float specularStrength = pow(max(dot(cameraDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = specularIntensity * specularStrength * light.specular * vec3(texture(material.specular, vTexCoord));

    return (ambient + diffuse + specular);
}

// 计算点光源产生的颜色
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 cameraPos){
    vec3 ambientIntensity = vec3(0.1f); 
    vec3 ambient = ambientIntensity * light.ambient * vec3(texture(material.diffuse, vTexCoord));

    vec3 lightDir = normalize(light.pos - fragPos);
    vec3 diffuseIntensity = vec3(1.0f);
    float diffuseStrength = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diffuseIntensity * diffuseStrength * light.diffuse * vec3(texture(material.diffuse, vTexCoord));

    vec3 cameraDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specularIntensity = vec3(1.0f);
    float specularStrength = pow(max(dot(cameraDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = specularIntensity * specularStrength * light.specular * vec3(texture(material.specular, vTexCoord));

    float distance = length(light.pos - fragPos);
    float attenuation = 1.0f / (light.kc + light.kl * distance + light.kq * distance * distance);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

// 计算聚光灯源产生的颜色
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 cameraPos){
    vec3 color;
    float fragCosin = dot(normalize(fragPos - light.pos), normalize(light.dir));
    if(fragCosin > light.outerCutoff){
        float intensity = clamp((fragCosin - light.outerCutoff) / (light.innerCutOff - light.outerCutoff), 0.0f, 1.0f);

        vec3 ambientIntensity = vec3(0.1f); 
        vec3 ambient = ambientIntensity * light.ambient * vec3(texture(material.diffuse, vTexCoord));

        vec3 lightDir = normalize(light.pos - fragPos);
        vec3 diffuseIntensity = vec3(1.0f);
        float diffuseStrength = max(dot(normal, lightDir), 0.0f);
        vec3 diffuse = diffuseIntensity * diffuseStrength * light.diffuse * vec3(texture(material.diffuse, vTexCoord));

        vec3 cameraDir = normalize(cameraPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 specularIntensity = vec3(1.0f);
        float specularStrength = pow(max(dot(cameraDir, reflectDir), 0.0f), material.shininess);
        vec3 specular = specularIntensity * specularStrength * light.specular * vec3(texture(material.specular, vTexCoord));

        diffuse *= intensity;
        specular *= intensity;

        color = ambient + diffuse + specular;
    }else{
        vec3 ambientIntensity = vec3(0.2f); 
        vec3 ambient = ambientIntensity * light.ambient * vec3(texture(material.diffuse, vTexCoord));
        color = ambient;
    }

    return  color;
}