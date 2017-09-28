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


layout(std140) uniform Matrices{
    mat4 project;
    mat4 view;  
};
uniform mat4 transform;


//  Uniform Buffer Objects
//  默认内存布局为shared, 此时需要自己查询每个变量的偏移情况
//  std140内存布局通过std140规则定义变量在内存中的布局
//      std140布局规则，变量的偏移需要时下面基准的整数倍(N代表4个字节)
//      int,bool                    N
//      vector                      2N 或 4N (例如vec2为2N, vec3为4N, vec4为4N)
//      int,bool,vector等的数组      每个元素都为4N
//      Matrix                      列向量存储，每个4N
//      Struct                      根据上面的规则计算，最终取为4N的整数倍
//  OpenGL4.2或以上版本可以用下面的方法将UNiform Block绑定到Binding point
//  layout(std140,binding = 2) uniform Lights {};
//  在低版本中则需要使用下面的方法绑定
//  unsigned int index = glGetUniformBlockIndex(shaderId, UniformBlockName);
//  glUniformBlockBinding(shaderId, index, bindingPoint);

//  若要将C++程序中的uniform buffer绑定到binding point, 可以通过glBindBufferBase或glBindBufferRange实现
//  glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, UBO);
//  或
//  glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, UBO, offset, size);

void main(void){
    vTexCoord = iTexCoord;
    //vNormal = iNormal;

    // 修正旋转和不一致的缩放导致的法向量不对应的问题
    // 这种修正最好在CPU上完成，再通过uniform传到shader中，从而减小shader计算的开销
    vNormal = mat3(transpose(inverse(transform))) * iNormal;
    vFragPos = vec3(transform * vec4(iPos, 1.0f));
    gl_Position = project * view * transform * vec4(iPos, 1.0f);
}