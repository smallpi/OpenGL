/**
 *	author: smallpi
 *	date: 2017-08-20		
 *	description: 
 *		将两个纹理混合贴到一个矩形上
 *		绘制两个矩形
 *		一个矩形随着时间旋转
 *		另一个矩形随着时间缩放
 *
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
unsigned int VBO;
unsigned int texture0;
unsigned int texture1;
unsigned int EBO;
GLuint shaderId;

// 初始化函数
void onInit(void){
    //初始化OpenGL环境参数
    glClearColor(0.2f,0.5f,0.4f,1.0f);
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
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    // VertexArrays的创建只是为了简化流程
    glGenVertexArrays(1, &VAO);
    // 创建一个VBO用于存储有序的顶点数据
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);
    //另VBO的顶点部分数据指向shader的location=0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    //另VBO的纹理部分数据指向shader的location=0
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    glBindVertexArray(0);


    // 创建纹理并加载纹理
    glGenTextures(1,&texture0);
    glBindTexture(GL_TEXTURE_2D,texture0);

    int width,height,nChannel;
    stbi_set_flip_vertically_on_load(true); //自动像素上下翻转
    unsigned char* data = stbi_load("./images/wall.png",&width,&height,&nChannel,0);
    if(data){
        //第7个参数时图像的颜色格式
        //第3个参数才是需要存储的颜色格式
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        fprintf(stderr,"Fail to load Texture");
    }
    stbi_image_free(data);

    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);
    data = stbi_load("./images/smile.jpg",&width,&height,&nChannel,0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        fprintf(stderr,"Fail to load Texture");
    }

    // 初始化纹理设置
    // 纹理坐标超出0~1范围的怎么处理
    //		GL_REPEAT	重复
    //		GL_MIRRORED_REPEAT	重复镜像
    //		GL_CLAMP_TO_EDGE	和边缘相同
    //		GL_CLAMP_TO_BORDER	和指定的边缘颜色相同
    //			通过GL_TEXTURE_BORDER_COLOR设定
    //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // 纹理的取样方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 设置mipmap时的取样方式
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 初始化Shader
    vector<GLuint> shaderList;
    shaderList.push_back(Shader::createShader(GL_VERTEX_SHADER,"./default_vert.glsl"));
    shaderList.push_back(Shader::createShader(GL_FRAGMENT_SHADER,"./default_frag.glsl"));
    shaderId = Shader::createProgram(shaderList);

}

// 绘制函数
// 参数:
//     window : GLFW的窗口指针	
void onDraw(GLFWwindow* window){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderId);


    // 向shader传递变换矩阵
    // 可以理解为将模型从局部坐标系转换到世界坐标系
    mat4 transform(1.0f);
    mat::rotate(transform,glfwGetTime(),0.0f,0.0f,1.0f);
    mat::translate(transform,-0.5f,0.5f,0.0f);
    // shader的矩阵数据数据存储顺序是列优先的，所以第3个参数为true,即对矩阵进行转置
    glUniformMatrix4fv(glGetUniformLocation(shaderId,"transform"),1,true,transform.data());

    // texture0绑定到texture单元0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture0);
    // texture1绑定到texture单元1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,texture1);
    //再将texture单元0和单元1分别绑定到shader的sampler2D
    glUniform1i(glGetUniformLocation(shaderId,"Texture0"),0);
    glUniform1i(glGetUniformLocation(shaderId,"Texture1"),1);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);


    // 绘制第二个矩形
    // 第二个矩形的变换矩阵
    transform = mat4(1.0f);
    mat::scale(transform,2*sin(glfwGetTime()),2*sin(glfwGetTime()),1.0f);
    mat::translate(transform,0.5f,-0.5f,0.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderId,"transform"),1,true,transform.data());

    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

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
}

// 窗口大小变化处理函数
// 参数:
//     window : GLFW的窗口指针
//     width : 窗口宽度
//     height : 窗口高度
void onSize(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
    onDraw(window);
}

// 鼠标移动处理函数
// 参数:
//     window : GLFW的窗口指针
//     x : 鼠标坐在的x坐标
//     y : 鼠标所在的y坐标
void onMouseMove(GLFWwindow* window, double x, double y){
    
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
    window = glfwCreateWindow(640, 480, "OpenGL's road - scale and rotate rectangle", NULL, NULL);
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

    // 把该窗口设置成当前的OpenGL环境
    glfwMakeContextCurrent(window);
    // 自己写的初始化函数
    onInit();

    // 窗口循环直到关闭窗口
    while (!glfwWindowShouldClose(window))
    {
        //自己写的绘制函数
        onDraw(window);

        //事件队列处理
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}