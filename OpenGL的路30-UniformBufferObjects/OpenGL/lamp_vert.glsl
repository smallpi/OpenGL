#version 330 core

//顶点坐标数据
layout(location=0) in vec3 iPos;
//纹理坐标数据
layout(location=1) in vec2 iTexCoord;

layout(std140) uniform Matrices{
    mat4 project;
    mat4 view;
};
uniform mat4 transform;

void main(void){
    gl_Position = project * view * transform * vec4(iPos, 1.0f);
}