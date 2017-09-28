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

out vec4 oColor;

uniform bool isBox;
uniform Material material;
uniform PointLight pointLight;
uniform vec3 cameraPos;
uniform float farPlane;
// uniform vec3 lightPos;
uniform samplerCube cubeDepthmap;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 cameraPos);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 cameraPos);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 cameraPos);


float calcShadow(vec3 fragPos){
    vec3 cubeTexCoord = fragPos - pointLight.pos;
    float currentDepth = length(cubeTexCoord);

    float shadow = 0.0;
    float bias = 0.1;

    // PCF 一种减少阴影锯齿的方法。
    // 通过在一个像素点出进行多次采样
    // 但是循环总共有64次
    // float samples = 4.0;
    // float offset = 0.1;
    // for(float x=-offset; x<offset; x+=offset/(samples*0.5)){
    //     for(float y=-offset; y<offset; y+=offset/(samples*0.5)){
    //         for(float z=-offset; z<offset; z+=offset/(samples*0.5)){
    //             float closestDepth = texture(cubeDepthmap, cubeTexCoord + vec3(x,y,z)).x;
    //             closestDepth *= farPlane;
    //             if(currentDepth - bias > closestDepth){
    //                 shadow += 1.0;
    //             }
    //         }
    //     }
    // }
    // shadow /= (samples * samples * samples);


    // PCF减少循环次数的方法
    vec3 sampleOffsetDirections[20] = vec3[]
    (
        vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
        vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
        vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
        vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
        vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );
    int samples = 20;
    // float diskRadius = 0.1;
    // 根据眼睛到物体的距离调整阴影的柔和度
    float viewDistance = length(vFragPos - cameraPos);
    float diskRadius = (1.0 + viewDistance/farPlane) / 25.0;
    for(int i=0; i<samples; i++){
        float closestDepth = texture(cubeDepthmap, cubeTexCoord + sampleOffsetDirections[i] * diskRadius).x;
        closestDepth *= farPlane;
        if(currentDepth - bias > closestDepth){
            shadow += 1.0;
        }
    }
    shadow /= samples;

    // float cloestDepth = texture(cubeDepthmap, cubeTexCoord).x;
    // cloestDepth = cloestDepth * farPlane;
   
    // float shadow = currentDepth - bias < cloestDepth ? 0.0f : 1.0f;
    return shadow;
}

// 主函数
void main(void){

    float shadow = calcShadow(vFragPos);

    vec3 normal = normalize(vNormal);
    if(isBox){
        normal = -normal;
    }
    vec3 color = texture(material.diffuse, vTexCoord).rgb;
    vec3 lightColor = vec3(1.0);

    // ambient
    vec3 ambient = 0.3 * lightColor;
    // diffuse
    vec3 lightDir = normalize(pointLight.pos - vFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    //specular
    vec3 viewDir = normalize(cameraPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * lightColor;

    // vec3 ambientIntensity = vec3(0.5f); 
    // vec3 ambient = ambientIntensity * pointLight.ambient * vec3(texture(material.diffuse, vTexCoord));

    // vec3 lightDir = normalize(pointLight.pos - vFragPos);
    // vec3 diffuseIntensity = vec3(1.0f);
    // float diffuseStrength = max(dot(normal, lightDir), 0.0f);
    // vec3 diffuse = diffuseIntensity * diffuseStrength * pointLight.diffuse * vec3(texture(material.diffuse, vTexCoord));

    // vec3 cameraDir = normalize(cameraPos - vFragPos);
    // vec3 reflectDir = reflect(-lightDir, normal);
    // vec3 specularIntensity = vec3(1.0f);
    // float specularStrength = pow(max(dot(cameraDir, reflectDir), 0.0f), material.shininess);
    // vec3 specular = specularIntensity * specularStrength * pointLight.specular * vec3(texture(material.specular, vTexCoord));

    // float distance = length(pointLight.pos - vFragPos);
    // float attenuation = 1.0f / (pointLight.kc + pointLight.kl * distance + pointLight.kq * distance * distance);
    // ambient *= attenuation;
    // diffuse *= attenuation;
    // specular *= attenuation;

    // vec3 finalColor = ambient + (1.0f - shadow) * (diffuse + specular);
    
    
    vec3 finalColor = (ambient + (1.0 - shadow) * (0.5*diffuse + 0.5*specular)) * color;

    // Gamma修正
    // float gamma = 2.2f;
    // finalColor = pow(finalColor, vec3(1.0f / gamma));


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