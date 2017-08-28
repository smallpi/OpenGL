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

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform vec3 cameraPos;
// 真正的texture像素数据
//uniform sampler2D Texture0;
//uniform sampler2D Texture1;


vec3 calcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 cameraPos);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 cameraPos);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 cameraPos);


void main(void){
    //oColor = mix(texture(Texture0,vTexCoord),texture(Texture1,vTexCoord),0.3);

    // vec3 ambientIntensity = vec3(1.0f); // material.ambient中已经把ambientIntensity计算进去了
    // vec3 ambient = ambientIntensity * light.ambient * vec3(texture(material.diffuse, vTexCoord));

    // vec3 normal = normalize(vNormal);
    // vec3 lightDir = normalize(light.pos - vFragPos);
    // vec3 diffuseIntensity = vec3(1.0f);
    // float diffuseStrength = max(dot(normal, lightDir), 0.0f);
    // vec3 diffuse = diffuseIntensity * diffuseStrength * light.diffuse * vec3(texture(material.diffuse, vTexCoord));

    // vec3 cameraDir =  normalize(cameraPos - vFragPos);
    // vec3 reflectDir = reflect(-lightDir, normal);
    // vec3 specularIntensity = vec3(1.0f);
    // float specularStrength = pow(max(dot(cameraDir, reflectDir), 0.0f), material.shininess);
    // vec3 specular = specularIntensity * specularStrength * light.specular * vec3(texture(material.specular, vTexCoord));

    // vec3 finalColor = (ambient + diffuse + specular);

    vec3 finalColor = calcDirLight(dirLight, vNormal, vFragPos, cameraPos);
    finalColor += calcPointLight(pointLight, vNormal, vFragPos, cameraPos);
    oColor = vec4(finalColor, 1.0f);

    // float near = 0.1f;
    // float far = 100.0f;
    // 根据下面两步计算可以把gl_FragCoord.z这个深度值计算到near~far之间
    // 线性分布，即viewspace中的z值
    // float trueDepth = gl_FragCoord.z * 2.0f - 1.0f;
    // trueDepth = (2.0f * near * far) / (far + near - trueDepth * (far - near));
    // trueDepth = trueDepth / far;
    // oColor = vec4(vec3(trueDepth), 1.0f);
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