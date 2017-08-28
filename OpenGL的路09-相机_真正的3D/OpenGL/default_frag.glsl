#version 330 core

//vertex shader传递过来的纹理坐标数据
in vec2 vTexCoord;

out vec4 oColor;

// 真正的texture像素数据
uniform sampler2D Texture0;
uniform sampler2D Texture1;

void main(void){
    oColor = mix(texture(Texture0,vTexCoord),texture(Texture1,vTexCoord),0.3);
    //oColor = vec4(vColor,1.0f);
}