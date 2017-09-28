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

uniform mat4 transform;
uniform mat4 view;
uniform mat4 project;

void main(void){
    vTexCoord = iTexCoord;
    vFragPos = vec3(transform * vec4(iPos,1.0f));
    vNormal = mat3(transpose(inverse(transform))) * iNormal;

    gl_Position = project * view * vec4(vFragPos, 1.0f);
}