/**
 *	author: smallpi
 *	date: 2017-08-20		
 *	description: 
 *		通过Blinn-Phone Shading改善Phone-Shading中由于specular的角度可能大于90度的问题
 *		Blinn-Phone Shading采用视线和光线的中间向量，与片元的法向量进行比较
 *		而Phone-Shading中的Specular通过视线的反射向量和光线向量进行比较
 *		通过按键B和P，切换Blinn-Phone shading和Phone shading
 **/

/**
 *	依赖项 
 *		GLFW3.2.1 (用于创建窗口)
 *			lib: glfw3.lib
 *			dll: glfw3.dll
 *		GLEW0.96 (用于使用高版本的OpenGL API)
 *			lib: glew32.lib
 *			dll: glew32.dll
 *		stb_image
 *			stbimage.h
 **/


//必须导入glew.h 再导入gl.h
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "Shader.h" // 编译shader的辅助库 参考learnopengl.com
#include "mat.h" // 自己写的一个矩阵类，提供了平移旋转缩放的方法

#include <stdio.h>


unsigned int VAO;
unsigned int lampVAO;
unsigned int VBO;
unsigned int texture0;
unsigned int texture1;
GLuint shaderId;
GLuint lampShaderId;

// 相机的参数
// 传递给lookAt
float cameraPos[3] = {0.0f, -10.0f, 0.0f};
float cameraFront[3] = {0.0f, 1.0f, 0.0f};
float cameraUp[3] = {0.0f, 0.0f, 1.0f};
// 物体颜色和光照颜色
float objectColor[3] = {1.0f, 0.8f, 0.1f};
float lightColor[3] = {1.0f,1.0f,1.0f};
// 光照的位置
float lightPos[3] = {2.0f,-1.5f,1.2f};
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 鼠标最后的位置
float lastX = 320.0f;
float lastY = 240.0f;
// 欧拉水平角和俯仰角
float yaw = 0.0f;
float pitch = 0.0f;
bool firstMove = true; //处理第一次移动鼠标快闪的问题

float aspect = 1.3333f;

int blinnPhone = 1;

//float cubePosition[10][3] =
//{
//    {0.0f,  0.0f,  0.0f},
//    {2.0f,  5.0f, -15.0f},
//    {-1.5f, -2.2f, -2.5f},  
//    {-3.8f, -2.0f, -12.3f},  
//    { 2.4f, -0.4f, -3.5f},  
//    {-1.7f,  3.0f, -7.5f},  
//    { 1.3f, -2.0f, -2.5f},  
//    { 1.5f,  2.0f, -2.5f}, 
//    { 1.5f,  0.2f, -1.5f}, 
//    {-1.3f,  1.0f, -1.5f}
//};

// 初始化函数
void onInit(void){
    //初始化OpenGL环境参数
    glClearColor(0.1f,0.5f,0.4f,1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    //渐变色要指定为GL_SMOOTH(默认),否则为GL_FLAT
    glShadeModel(GL_SMOOTH);

    // 初始化GLEW库
    if(glewInit() != GLEW_OK){
        exit(-1);
    }

    //初始化数据
    //矩形的顶点数据 和 纹理坐标
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
    };
    // VertexArrays的创建只是为了简化流程
    glGenVertexArrays(1, &VAO);
    // 创建一个VBO用于存储有序的顶点数据
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);
    //另VBO的顶点部分数据指向shader的location=0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    //另VBO的纹理部分数据指向shader的location=1
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    //另VBO的法向量部分数据指向shader的location=2
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));


    glBindVertexArray(0);

    glGenVertexArrays(1,&lampVAO);
    glBindVertexArray(lampVAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));

    glBindVertexArray(0);
    // 创建纹理并加载纹理
    //glGenTextures(1,&texture0);
    //glBindTexture(GL_TEXTURE_2D,texture0);

    //int width,height,nChannel;
    //stbi_set_flip_vertically_on_load(true); //自动像素上下翻转
    //unsigned char* data = stbi_load("./images/wall.png",&width,&height,&nChannel,0);
    //if(data){
    //    //第7个参数时图像的颜色格式
    //    //第3个参数才是需要存储的颜色格式
    //    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //}else{
    //    fprintf(stderr,"Fail to load Texture");
    //}
    //stbi_image_free(data);

    //glGenTextures(1,&texture1);
    //glBindTexture(GL_TEXTURE_2D,texture1);
    //data = stbi_load("./images/smile.jpg",&width,&height,&nChannel,0);
    //if(data){
    //    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //}else{
    //    fprintf(stderr,"Fail to load Texture");
    //}

    // 初始化纹理设置
    // 纹理坐标超出0~1范围的怎么处理
    //		GL_REPEAT	重复
    //		GL_MIRRORED_REPEAT	重复镜像
    //		GL_CLAMP_TO_EDGE	和边缘相同
    //		GL_CLAMP_TO_BORDER	和指定的边缘颜色相同
    //			通过GL_TEXTURE_BORDER_COLOR设定
    //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // 纹理的取样方式
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 设置mipmap时的取样方式
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 初始化Shader
    vector<GLuint> shaderList;
    shaderList.push_back(Shader::createShader(GL_VERTEX_SHADER,"./default_vert.glsl"));
    shaderList.push_back(Shader::createShader(GL_FRAGMENT_SHADER,"./default_frag.glsl"));
    shaderId = Shader::createProgram(shaderList);
    // 初始化光源正方体的shader
    vector<GLuint> lightShaderList;
    lightShaderList.push_back(Shader::createShader(GL_VERTEX_SHADER,"./lamp_vert.glsl"));
    lightShaderList.push_back(Shader::createShader(GL_FRAGMENT_SHADER,"./lamp_frag.glsl"));
    lampShaderId = Shader::createProgram(lightShaderList);
}

// 绘制函数
// 参数:
//     window : GLFW的窗口指针	
void onDraw(GLFWwindow* window){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderId);


    // 根据欧拉水平角和俯仰角计算cameraFront
    cameraFront[0] = cos(TO_RADIAN(pitch)) * cos(TO_RADIAN(yaw+90.0f));
    cameraFront[1] = cos(TO_RADIAN(pitch)) * sin(TO_RADIAN(yaw+90.0f));
    cameraFront[2] = sin(TO_RADIAN(pitch));

    // 向shader传递变换矩阵
    // world标系转换到view坐标系
    mat4 viewMat;
    mat::lookAt(viewMat,
        cameraPos[0],cameraPos[1],cameraPos[2],
        cameraPos[0]+cameraFront[0],cameraPos[1]+cameraFront[1],cameraPos[2]+cameraFront[2],
        cameraUp[0],cameraUp[1],cameraUp[2]);
    glUniformMatrix4fv(glGetUniformLocation(shaderId,"view"),1,true,viewMat.data());
    // view坐标系到clip坐标系
    mat4 projectMat;
    mat::perspective(projectMat,TO_RADIAN(45),aspect,0.1f,100.0f);
    //mat::ortho(projectMat,-2,2,-2,2,0.1,20);
    glUniformMatrix4fv(glGetUniformLocation(shaderId,"project"),1,true,projectMat.data());

    // texture0绑定到texture单元0
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D,texture0);
    // texture1绑定到texture单元1
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D,texture1);
    //再将texture单元0和单元1分别绑定到shader的sampler2D
    //glUniform1i(glGetUniformLocation(shaderId,"Texture0"),0);
    //glUniform1i(glGetUniformLocation(shaderId,"Texture1"),1);

    mat4 transform(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderId,"transform"),1,true,transform.data());

    // 向shader传递光照颜色和物体颜色
    glUniform3fv(glGetUniformLocation(shaderId,"objectColor"),1,objectColor);
    glUniform3fv(glGetUniformLocation(shaderId,"lightColor"),1,lightColor);
    // 向shader传递光源位置和相机位置
    glUniform3fv(glGetUniformLocation(shaderId,"lightPos"),1,lightPos);
    glUniform3fv(glGetUniformLocation(shaderId,"cameraPos"),1,cameraPos);
	glUniform1i(glGetUniformLocation(shaderId,"blinnPhone"),blinnPhone);


    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,36);
    

    // 绘制光源立方体
    glUseProgram(lampShaderId);
    
    glUniformMatrix4fv(glGetUniformLocation(lampShaderId,"project"),1,true,projectMat.data());
    glUniformMatrix4fv(glGetUniformLocation(lampShaderId,"view"),1,true,viewMat.data());
    transform = mat4(1.0f);
    mat::scale(transform,0.2f,0.2f,0.2f);
    mat::translate(transform,lightPos[0],lightPos[1],lightPos[2]);
    glUniformMatrix4fv(glGetUniformLocation(lampShaderId,"transform"),1,true,transform.data());

    glBindVertexArray(lampVAO);
    glDrawArrays(GL_TRIANGLES,0,36);

    glfwSwapBuffers(window);
}

// 键盘处理函数
// 参数:
//     window : GLFW的窗口指针
//     key : 按下什么键
void onKey(GLFWwindow* window, int key, int scanCode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window,GLFW_TRUE);
    }
	if(key == GLFW_KEY_B && action == GLFW_PRESS){
		blinnPhone = 1;
	}else if(key == GLFW_KEY_P && action == GLFW_PRESS){
		blinnPhone = 0;
	}
}

// 窗口大小变化处理函数
// 参数:
//     window : GLFW的窗口指针
//     width : 窗口宽度
//     height : 窗口高度
void onSize(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
    aspect = width / height;

    onDraw(window);
}

// 鼠标移动处理函数
// 参数:
//     window : GLFW的窗口指针
//     x : 鼠标坐在的x坐标
//     y : 鼠标所在的y坐标
void onMouseMove(GLFWwindow* window, double x, double y){
    
    //处理第一次移动鼠标快闪的问题
    if(firstMove){
        lastX = x;
        lastY = y;
        firstMove = false;
    }

    float xoffset = x - lastX;
    float yoffset = y - lastY;
    lastX = x;
    lastY = y;

    yaw -= 0.1f * xoffset;
    pitch -= 0.1f * yoffset;

    if(pitch > 89.0f){
        pitch = 89.0f;
    }
    if(pitch < -89.0f){
        pitch = -89.0f;
    }
}

// 鼠标滚轮梳理函数
void onMouseWheel(GLFWwindow* window, double xoffset, double yoffset){
    
}

// 鼠标按键处理函数

void onMouseButton(GLFWwindow* window, int button, int action, int mods){
    switch(button){
    case GLFW_MOUSE_BUTTON_LEFT:	
        {
            //鼠标左键处理
        }
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        {
            //鼠标中键处理
        }
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        {
            //鼠标右键处理
        }
        break;
    }
}

// 处理按住键盘事件
void processKeyInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }
    float cameraSpeed = deltaTime * 4.0f;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        cameraPos[0] += cameraSpeed * cameraFront[0];
        cameraPos[1] += cameraSpeed * cameraFront[1];
        cameraPos[2] += cameraSpeed * cameraFront[2];
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        cameraPos[0] -= cameraSpeed * cameraFront[0];
        cameraPos[1] -= cameraSpeed * cameraFront[1];
        cameraPos[2] -= cameraSpeed * cameraFront[2];
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        float cameraRight[3] = {
            cameraFront[1] * cameraUp[2] - cameraFront[2] * cameraUp[1],
            cameraFront[2] * cameraUp[0] - cameraFront[0] * cameraUp[2],
            cameraFront[0] * cameraUp[1] - cameraFront[1] * cameraUp[0],
        };
        float length = sqrt(cameraRight[0]*cameraRight[0]+cameraRight[1]*cameraRight[1]+cameraRight[2]*cameraRight[2]);
        cameraRight[0] /= length;
        cameraRight[1] /= length;
        cameraRight[2] /= length;
        cameraPos[0] -= cameraSpeed * cameraRight[0]; 
        cameraPos[1] -= cameraSpeed * cameraRight[1];
        cameraPos[2] -= cameraSpeed * cameraRight[2];
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        float cameraRight[3] = {
            cameraFront[1] * cameraUp[2] - cameraFront[2] * cameraUp[1],
            cameraFront[2] * cameraUp[0] - cameraFront[0] * cameraUp[2],
            cameraFront[0] * cameraUp[1] - cameraFront[1] * cameraUp[0],
        };
        float length = sqrt(cameraRight[0]*cameraRight[0]+cameraRight[1]*cameraRight[1]+cameraRight[2]*cameraRight[2]);
        cameraRight[0] /= length;
        cameraRight[1] /= length;
        cameraRight[2] /= length;
        cameraPos[0] += cameraSpeed * cameraRight[0]; 
        cameraPos[1] += cameraSpeed * cameraRight[1];
        cameraPos[2] += cameraSpeed * cameraRight[2];
    }

}

// 错误处理函数
//     参数
//     error : 错误代码
//     description: 错误说明
void onError( int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
}


// main函数 程序的入口
int main(int argc, char** argv){
    GLFWwindow* window;

    // 初始化GLFW库
    if (!glfwInit()){
        return -1;
    }


    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //设置最高的OpenGL版本
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0); //设置最低的OpenGL版本
    // 创建windows mode的窗口
    // 并创建OpenGL环境
    window = glfwCreateWindow(640, 480, "OpenGL's road -Blinn-Phone shading and Phone shading", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetErrorCallback(onError); //设置错误处理函数
    glfwSetKeyCallback(window,onKey); //设置键盘处理函数
    glfwSetFramebufferSizeCallback(window,onSize); //设置窗口大小变化处理函数
    glfwSetCursorPosCallback(window,onMouseMove); //设置鼠标移动处理函数
    glfwSetScrollCallback(window,onMouseWheel); //设置鼠标滚轮处理函数
    glfwSetMouseButtonCallback(window,onMouseButton); //设置鼠标案卷处理函数

    //隐藏鼠标，但可以捕获鼠标事件
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 把该窗口设置成当前的OpenGL环境
    glfwMakeContextCurrent(window);
    // 自己写的初始化函数
    onInit();

    // 窗口循环直到关闭窗口
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processKeyInput(window);

        //自己写的绘制函数
        onDraw(window);

        //事件队列处理
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}