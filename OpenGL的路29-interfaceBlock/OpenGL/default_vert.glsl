#version 330 core

//顶点坐标数据
layout(location=0) in vec3 iPos;
//纹理坐标数据
layout(location=1) in vec2 iTexCoord;
//法向量数据
layout(location=2) in vec3 iNormal;

out VS_OUT{
    vec2 texCoord;
    vec3 normal;
    vec3 fragPos;
} vs_out;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 project;

void main(void){
    vs_out.texCoord = iTexCoord;
    //vNormal = iNormal;

    // 修正旋转和不一致的缩放导致的法向量不对应的问题
    // 这种修正最好在CPU上完成，再通过uniform传到shader中，从而减小shader计算的开销
    vs_out.normal = mat3(transpose(inverse(transform))) * iNormal;
    vs_out.fragPos = vec3(transform * vec4(iPos, 1.0f));
    gl_Position = project * view * transform * vec4(iPos, 1.0f);
}