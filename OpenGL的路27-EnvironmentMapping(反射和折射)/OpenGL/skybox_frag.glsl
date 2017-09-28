#version 330 core

in vec3 vSkyboxTexCoord;

out vec4 oColor;

uniform samplerCube skyboxTexture;

void main(void){
    oColor = texture(skyboxTexture, vSkyboxTexCoord);
}