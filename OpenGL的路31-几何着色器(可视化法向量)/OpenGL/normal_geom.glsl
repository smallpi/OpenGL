#version 330 core

// 首先需要定义输入进来的几何图形
// points               使用GL_POINTS时
// lines                使用GL_LINES或GL_LINE_STRIP时
// lines_adjacency      使用GL_LINES_ADJACENCY或GL_LINE_STRIP_ADJACENCY时
// triangles            使用GL_TRIANGLES或GL_TRIANGLE_STRIP或GL_TRIANGLE_FAN时
// triangles_adjacnecy  使用GL_TRIANGLES_ADJACENCY或GL_TRIANGLE_STRIP_ADJACENCY时
layout(triangles) in;

// 然后需要定义输出的几何图形
// 以及通过max_vertices指定最大输出的顶点数
// 输出的几何图形可以为
// points
// line_strip
// triangle_strip
layout(line_strip, max_vertices = 6) out;

// 从VertexShader输入的数据，为数组
in vec3 vNormal[];

// 运用几何着色器时，顶点的变换可以在几何着色器完成
// uniform mat4 transform;
// uniform mat4 view;
// uniform mat4 project;

// Geometry Shader有个内置的输入数组gl_in[];
// in gl_Vertex
// {
//     vec4  gl_Position;
//     float gl_PointSize;
//     float gl_ClipDistance[];
// } gl_in[];  

void main(void){
    
    vec3 normal = normalize(vNormal[0]);//getNormal();
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(normal * 0.2f, 0.0f);
    EmitVertex();
    EndPrimitive();

    normal = normalize(vNormal[1]);//getNormal();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position +vec4(normal * 0.2f, 0.0f);
    EmitVertex();
    EndPrimitive();

    normal = normalize(vNormal[2]);//getNormal();
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position + vec4(normal * 0.2f, 0.0f);
    EmitVertex();
    EndPrimitive();
}