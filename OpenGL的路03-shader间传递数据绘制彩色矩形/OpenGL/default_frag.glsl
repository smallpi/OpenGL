#version 330 core

//vertex shader传递过来的颜色数据
in vec3 vColor;

out vec4 oColor;

void main(void){
    oColor = vec4(vColor,1.0f);
}