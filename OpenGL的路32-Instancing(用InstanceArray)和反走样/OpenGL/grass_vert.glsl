#version 330 core

//顶点坐标数据
layout(location=0) in vec3 iPos;
//纹理坐标数据
layout(location=1) in vec2 iTexCoord;
//法向量数据
layout(location=2) in vec3 iNormal;

// Instance Array
// 当Instance Array为mat4类型时
// 需要当成4个vec4类型，且VertexAttrib需要留4个
layout(location=3) in vec3 iOffset;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vFragPos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 project;

void main(void){
    vTexCoord = iTexCoord;
    //vNormal = iNormal;

    // 修正旋转和不一致的缩放导致的法向量不对应的问题
    // 这种修正最好在CPU上完成，再通过uniform传到shader中，从而减小shader计算的开销
    vNormal = mat3(transpose(inverse(transform))) * iNormal;
    vFragPos = vec3(transform * vec4(iPos, 1.0f));

    vec3 pos = iPos;
    pos.z += 0.5f; 
    pos *= (2.5f + 2.0f * sin(gl_InstanceID));
    pos += iOffset;

    gl_Position = project * view * transform * vec4(pos, 1.0f);
}