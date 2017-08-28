#version 330 core

//顶点坐标数据
layout(location=0) in vec3 iPos;
//纹理坐标数据
layout(location=1) in vec2 iTexCoord;
//法向量数据
layout(location=2) in vec3 iNormal;

out vec2 vTexCoord;
out vec3 vColor;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 project;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main(void){
    vTexCoord = iTexCoord;

    // 修正旋转和不一致的缩放导致的法向量不对应的问题
    // 这种修正最好在CPU上完成，再通过uniform传到shader中，从而减小shader计算的开销
    // vNormal = mat3(transpose(inverse(transform))) * iNormal;
    
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 normal = normalize(iNormal);
    vec3 fragPos = vec3(transform * vec4(iPos, 1.0f));
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuseStrength = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diffuseStrength * lightColor;

    vec3 cameraDir =  normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularIntensity = 0.5f;
    float specularStrength = specularIntensity * pow(max(dot(cameraDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * lightColor;

    vColor = (ambient + diffuse + specular) * objectColor;


    gl_Position = project * view * transform * vec4(iPos, 1.0f);
}