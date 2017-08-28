#version 330 core

//顶点坐标数据
layout(location=0) in vec3 iPos;
//纹理坐标数据
layout(location=1) in vec2 iTexCoord;

out vec2 vTexCoord;


void main(void){
    vTexCoord = iTexCoord;
    gl_Position = vec4(iPos, 1.0f);
}