#version 330 core

//顶点数据
layout(location=0) in vec3 iPos;

void main(void){
    gl_Position = vec4(iPos, 1.0f);
}