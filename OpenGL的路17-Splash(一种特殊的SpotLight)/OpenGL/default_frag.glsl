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
    //sampler2D emission; // emission maps;
    float shininess;
};


// 光照结构体
struct Light{
    vec3 pos;
    vec3 dir;
    float innerCutOff;
    float outerCutOff;
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
uniform Light light;
uniform vec3 cameraPos;
// 真正的texture像素数据
//uniform sampler2D Texture0;
//uniform sampler2D Texture1;

void main(void){
    //oColor = mix(texture(Texture0,vTexCoord),texture(Texture1,vTexCoord),0.3);

    vec3 ambientIntensity = vec3(0.2f); // material.ambient中已经把ambientIntensity计算进去了
    vec3 ambient = ambientIntensity * light.ambient * vec3(texture(material.diffuse, vTexCoord));

    float fragCosin = dot(normalize(vFragPos - light.pos), light.dir);
    vec3 finalColor;
    if(fragCosin > light.outerCutOff){
        // 在外圈之内都要计算所有光照
        float intensity = clamp((fragCosin - light.outerCutOff) / (light.innerCutOff - light.outerCutOff), 0.0f, 1.0f);

        vec3 normal = normalize(vNormal);
        vec3 lightDir = normalize(light.pos - vFragPos);
        vec3 diffuseIntensity = vec3(1.0f);
        float diffuseStrength = max(dot(normal, lightDir), 0.0f);
        vec3 diffuse = diffuseIntensity * diffuseStrength * light.diffuse * vec3(texture(material.diffuse, vTexCoord));

        vec3 cameraDir =  normalize(cameraPos - vFragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 specularIntensity = vec3(1.0f);
        float specularStrength = pow(max(dot(cameraDir, reflectDir), 0.0f), material.shininess);
        vec3 specular = specularIntensity * specularStrength * light.specular * vec3(texture(material.specular, vTexCoord));

        diffuse *= intensity;
        specular *= intensity;


        finalColor = ambient + diffuse + specular;
    }else{
        finalColor = ambient;
    }
    
    oColor = vec4(finalColor, 1.0f);
}