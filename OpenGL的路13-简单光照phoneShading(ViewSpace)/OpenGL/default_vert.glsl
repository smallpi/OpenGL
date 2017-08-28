#version 330 core

//顶点坐标数据
layout(location=0) in vec3 iPos;
//纹理坐标数据
layout(location=1) in vec2 iTexCoord;
//法向量数据
layout(location=2) in vec3 iNormal;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vFragPos;
out vec3 vLightPos;
//out vec3 vCameraPos; // view space计算光照cameraPos为(0,0,0)

uniform mat4 transform;
uniform mat4 view;
uniform mat4 project;

uniform vec3 lightPos;
//uniform vec3 cameraPos; // view space计算光照cameraPos为(0,0,0)

void main(void){
    vTexCoord = iTexCoord;

    // 修正旋转和不一致的缩放导致的法向量不对应的问题
    // 这种修正最好在CPU上完成，再通过uniform传到shader中，从而减小shader计算的开销
    vNormal = mat3(transpose(inverse(view * transform))) * iNormal;
    vFragPos = vec3(view * transform * vec4(iPos, 1.0f));
    vLightPos = vec3(view * vec4(lightPos, 1.0f));
    //vCameraPos = vec3(view * vec4(cameraPos, 1.0f)); // view space计算光照cameraPos为(0,0,0)
    gl_Position = project * view * transform * vec4(iPos, 1.0f);
}