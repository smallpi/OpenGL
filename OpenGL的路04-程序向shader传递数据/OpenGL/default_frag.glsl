#version 330 core

//vertex shader传递过来的颜色数据
in vec3 vColor;

out vec4 oColor;

uniform float uColorRatio;

void main(void){
    oColor = vec4(vColor.x * uColorRatio, vColor.y, vColor.z, 1.0f);
}