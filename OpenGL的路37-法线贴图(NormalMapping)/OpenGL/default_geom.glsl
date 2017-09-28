#version 330 core

layout(triangles) in;

layout(triangle_strip, max_vertices = 3) out;

in vec2 vTexCoord[];
in vec3 vFragPos[];

out vec2 gTexCoord;
out vec3 gNormal;
out vec3 gFragPos;
out mat3 gTBN;


uniform mat4 transform;

void main(void){
    vec2 deltaUV1 = vTexCoord[1] - vTexCoord[0];
    vec2 deltaUV2 = vTexCoord[2] - vTexCoord[0];
    vec3 edge1 = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);
    vec3 edge2 = vec3(gl_in[2].gl_Position - gl_in[0].gl_Position);

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
    vec3 tangle;
    vec3 bitangle;
    
    tangle.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangle.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangle.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangle = normalize(tangle);

    bitangle.x = f * (deltaUV1.x * edge2.x - deltaUV2.x * edge1.x);
    bitangle.y = f * (deltaUV1.x * edge2.y - deltaUV2.x * edge1.y);
    bitangle.z = f * (deltaUV1.x * edge2.z - deltaUV2.x * edge1.z);
    bitangle = normalize(bitangle);

    vec3 normal = normalize(cross(edge1,edge2));

    tangle = normalize(vec3(transform * vec4(tangle, 0.0)));
    bitangle = normalize(vec3(transform * vec4(bitangle, 0.0)));
    vec3 N = normalize(vec3(transform * vec4(normal, 0.0)));

    

    gTexCoord = vTexCoord[0];
    gNormal = normal;
    gFragPos = vFragPos[0];
    gTBN = mat3(tangle, bitangle, N); 
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gTexCoord = vTexCoord[1];
    gFragPos = vFragPos[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gTexCoord = vTexCoord[2];
    gFragPos = vFragPos[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();

}