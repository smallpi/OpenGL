#version 330 core

//顶点坐标数据
layout(location=0) in vec3 iPos;
//纹理坐标数据
layout(location=1) in vec2 iTexCoord;

out vec2 vTexCoord;
out vec3 vFragPos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 project;

void main(void){
    vTexCoord = iTexCoord;
    vFragPos = vec3(transform * vec4(iPos,1.0f));

    gl_Position = project * view * vec4(vFragPos, 1.0f);
}