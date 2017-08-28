/**
 *	author: smallpi
 *	date: 2017-08-20		
 *	description: GLFW的简单配置
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
 **/


//必须导入glew.h 再导入gl.h
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>


// 初始化函数
void onInit(void){
	//初始化OpenGL
    glClearColor(0.2f,0.5f,0.4f,1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
	// 初始化GLEW库
	if(glewInit() != GLEW_OK){
		exit(-1);
	}
}

// 绘制函数
// 参数:
//     window : GLFW的窗口指针	
void onDraw(GLFWwindow* window){
    glClear(GL_COLOR_BUFFER_BIT);

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
    window = glfwCreateWindow(640, 480, "OpenGL's road - Setting up Environment", NULL, NULL);
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