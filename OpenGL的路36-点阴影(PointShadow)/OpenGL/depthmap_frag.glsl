#version 330 core

in vec4 gFragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main(void){
    float lightDistance = length(gFragPos.xyz - lightPos);
    // 把深度值从[0,farPlane] 映射到 [0,1]
    lightDistance = lightDistance / farPlane;

    gl_FragDepth = lightDistance;
}