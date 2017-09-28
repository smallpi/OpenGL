#version 330 core

//顶点坐标数据
layout(location=0) in vec3 iPos;
//纹理坐标数据
layout(location=1) in vec2 iTexCoord;
//法向量数据
layout(location=2) in vec3 iNormal;

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
    gl_Position = project * view * transform * vec4(iPos, 1.0f);

    //  VERTEX_SHADER
    //      gl_PointSize (out)点的大小 需要 glEnable(GL_PROGRAM_POINT_SIZE)
    //      gl_VertexID (in)当glDrawElements时为顶点的索引值，当glDrawArrays时为顶点的序号
    //      gl_InstanceID (in)当glDrawElementsInstance或glDrawArraysInstance时，它是instance的序号，从0开始，否则一直是0
    //      gl_DrawID (in)当Multi-draw时的ID
    //      gl_BaseVertex (in)
    //      gl_BaseInstance (in)
    //      gl_Position (out)
    //      gl_ClipDistance[] (out)

    //  FRAGMENT_SHADER
    //      gl_FragCoord (in vec4)
    //          layout(origin_upper_left) in vec4 gl_FragCoord; 调整gl_FragCoord的原点为屏幕左上角
    //          layout(pixel_center_integer) in vec4 gl_FragCoord; 默认时左下角原点像素中心为(0.5, 0.5),使用该布局将使得中心点为整数
    //      gl_FrontFacing (in bool)是否是正面的像素
    //      gl_PointCoord
}