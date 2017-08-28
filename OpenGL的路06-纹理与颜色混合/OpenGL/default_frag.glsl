#version 330 core

//vertex shader传递过来的颜色数据
in vec3 vColor;
//vertex shader传递过来的纹理数据
in vec2 vTexCoord;

out vec4 oColor;

// 真正的texture像素数据
uniform sampler2D Texture;

void main(void){
    oColor = texture(Texture,vTexCoord) * vec4(vColor,1.0);
    //oColor = vec4(vColor,1.0f);
}