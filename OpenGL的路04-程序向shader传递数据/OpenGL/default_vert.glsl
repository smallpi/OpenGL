#version 330 core

//顶点数据
layout(location=0) in vec3 iPos;
//颜色数据
layout(location=1) in vec3 iColor;

out vec3 vColor;

void main(void){
    vColor = iColor;
    gl_Position = vec4(iPos, 1.0f);
}