#version 330 core

layout(location=0) in vec3 iPos;
layout(location=1) in vec3 iTexCoord;

out vec3 vSkyboxTexCoord;


uniform mat4 view;
uniform mat4 project;

void main(void){
    
    //vSkyboxTexCoord = iTexCoord;
    vSkyboxTexCoord = iPos;
    gl_Position = project * view * vec4(iPos, 1.0f);
}