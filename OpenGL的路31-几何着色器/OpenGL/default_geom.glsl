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
layout(triangle_strip, max_vertices = 3) out;

// 从VertexShader输入的数据，为数组
in VS_OUT{
    vec2 texCoord;
    vec3 normal;
    vec3 fragPos;
} ge_in[];

// 输出的数据，不是数组
out vec2 gTexCoord;
out vec3 gNormal;
out vec3 gFragPos;

uniform float mag; // 爆炸的幅度
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


vec3 getNormal(){
    vec3 a = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
    return normalize(cross(a, b));
}
vec4 explode(vec4 position, vec3 normal){
    return position + vec4(normal * mag, 0.0f);
}

void main(void){
    
    vec3 normal = normalize(ge_in[0].normal);//getNormal();

    
    gNormal = normal;
    gTexCoord = ge_in[0].texCoord;
    gFragPos = ge_in[0].fragPos + ge_in[0].normal * mag;
    gl_Position = explode(gl_in[0].gl_Position, normal);
    EmitVertex();

    gNormal = normal;
    gTexCoord = ge_in[1].texCoord;
    gFragPos = ge_in[1].fragPos + ge_in[1].normal * mag;
    gl_Position = explode(gl_in[1].gl_Position, normal);
    EmitVertex();

    gNormal = normal;
    gTexCoord = ge_in[2].texCoord;
    gFragPos = ge_in[2].fragPos + ge_in[2].normal * mag;
    gl_Position = explode(gl_in[2].gl_Position, normal);
    EmitVertex();

    EndPrimitive();
}