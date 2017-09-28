#version 330 core

//顶点坐标数据
layout(location=0) in vec3 iPos;
//纹理坐标数据
layout(location=1) in vec2 iTexCoord;
//法向量数据
layout(location=2) in vec3 iNormal;

uniform mat4 transform;

void main(void){
    
    gl_Position = transform * vec4(iPos, 1.0f);
}