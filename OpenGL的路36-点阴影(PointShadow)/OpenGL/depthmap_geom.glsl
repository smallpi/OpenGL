#version 330 core

layout(triangles) in;

layout(triangle_strip, max_vertices=18) out;

out vec4 gFragPos;

uniform mat4 view[6];
uniform mat4 project;

void main(void){
    // 对于每一个三角形实体
    // 生成在六个面上的layer
    for(int face=0; face<6; face++){
        gl_Layer = face;
        for(int i=0; i<3; i++){
            gFragPos = gl_in[i].gl_Position;
            gl_Position = project * view[face] * gFragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}