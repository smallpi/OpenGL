#version 330 core

in vec2 vTexCoord;

out vec4 oColor;

uniform sampler2D texturePostProcess;

void main(void){

    // 反色调效果
    oColor = vec4(vec3(1.0f-texture(texturePostProcess, vTexCoord)), 1.0f);

    // 平均灰度化
    // vec4 fragColor = texture(texturePostProcess, vTexCoord);
    // float average = (fragColor.r + fragColor.g + fragColor.b) / 3.0f;
    // oColor = vec4(average, average, average, 1.0f);

    // 更接近人眼的灰度化
    // vec4 fragColor = texture(texturePostProcess, vTexCoord);
    // float average = 0.2126f * fragColor.r + 0.7152f * fragColor.g + 0.0722f * fragColor.b;
    // oColor = vec4(average, average, average, 1.0f);

    // 用核函数进行卷积 对图像进行处理, 提高对比度
    // 可以应用到麻醉场合
    // const float offset = 1.0f / 300.0f;
    // vec2 offsets[9] = vec2[](
    //     vec2(-offset, offset),
    //     vec2(0.0f, offset),
    //     vec2(offset, offset),
    //     vec2(-offset, 0.0f),
    //     vec2(0.0f, 0.0f),
    //     vec2(-offset, 0.0f),
    //     vec2(-offset, -offset),
    //     vec2(-offset, -offset),
    //     vec2(offset, -offset)
    // );

    // float kernel[9] = float[](
    //     -1, -1, -1,
    //     -1,  9, -1,
    //     -1, -1, -1
    // );

    // vec3 sampleTex[9];
    // for(int i=0; i<9; i++){
    //     sampleTex[i] = vec3(texture(texturePostProcess, vTexCoord + offsets[i]));
    // }

    // vec3 average = vec3(0.0f);
    // for(int i=0; i<9; i++){
    //     average += sampleTex[i] * kernel[i];
    // }
    // oColor = vec4(average, 1.0f);


    // 用核函数进行卷积 对图像进行处理, 图像模糊
    // 可应用于醉的场合，或带上眼睛的场合
    // const float offset = 1.0f / 300.0f;
    // vec2 offsets[9] = vec2[](
    //     vec2(-offset, offset),
    //     vec2(0.0f, offset),
    //     vec2(offset, offset),
    //     vec2(-offset, 0.0f),
    //     vec2(0.0f, 0.0f),
    //     vec2(-offset, 0.0f),
    //     vec2(-offset, -offset),
    //     vec2(-offset, -offset),
    //     vec2(offset, -offset)
    // );

    // float kernel[9] = float[](
    //     1.0f/16, 2.0f/16, 1.0f/16,
    //     2.0f/16, 4.0f/16, 2.0f/16,
    //     1.0f/16, 2.0f/16, 1.0f/16
    // );

    // vec3 sampleTex[9];
    // for(int i=0; i<9; i++){
    //     sampleTex[i] = vec3(texture(texturePostProcess, vTexCoord + offsets[i]));
    // }

    // vec3 average = vec3(0.0f);
    // for(int i=0; i<9; i++){
    //     average += sampleTex[i] * kernel[i];
    // }
    // oColor = vec4(average, 1.0f);



    // 用核函数进行卷积 对图像进行处理, 边缘提取
    // const float offset = 1.0f / 300.0f;
    // vec2 offsets[9] = vec2[](
    //     vec2(-offset, offset),
    //     vec2(0.0f, offset),
    //     vec2(offset, offset),
    //     vec2(-offset, 0.0f),
    //     vec2(0.0f, 0.0f),
    //     vec2(-offset, 0.0f),
    //     vec2(-offset, -offset),
    //     vec2(-offset, -offset),
    //     vec2(offset, -offset)
    // );

    // float kernel[9] = float[](
    //     1.0f,  1.0f, 1.0f,
    //     1.0f, -8.0f, 1.0f,
    //     1.0f,  1.0f, 1.0f
    // );

    // vec3 sampleTex[9];
    // for(int i=0; i<9; i++){
    //     sampleTex[i] = vec3(texture(texturePostProcess, vTexCoord + offsets[i]));
    // }

    // vec3 average = vec3(0.0f);
    // for(int i=0; i<9; i++){
    //     average += sampleTex[i] * kernel[i];
    // }
    // oColor = vec4(average, 1.0f);
}