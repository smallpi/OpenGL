#version 330 core

//顶点坐标数据
layout(location=0) in vec3 iPos;
//颜色数据
layout(location=1) in vec3 iColor;
//纹理坐标数据
layout(location=2) in vec2 iTexCoord;

out vec3 vColor;
out vec2 vTexCoord;


void main(void){
    vColor = iColor;
    vTexCoord = iTexCoord;
    gl_Position = vec4(iPos, 1.0f);
}