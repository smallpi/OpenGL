/**
 *	author: smallpi
 *	date: 2017-08-24		
 *	description: 
 *		自己创建一个framebuffer
 *		一个新的framebuffer至少绑定一个buffer(color,depth,stencil)
 *		而且colorbuffer的绑定是必须的。
 *		渲染到新的framebuffer称为离线渲染(off-line rendering)
 *		该framebuffer可以用于后置处理
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


#include "Shader.h" // 编译shader的辅助库 参考learnopengl.com
#include "Texture.h" // 自己写的Texture加载类
#include "mat.h" // 自己写的一个矩阵类，提供了平移旋转缩放的方法

#include <stdio.h>


unsigned int FBO; // 一个新的framebuffer
unsigned int RBO; // 用于新framebuffer的depth和stencil buffer

unsigned int VAO;
unsigned int postVAO;
unsigned int VBO;
unsigned int texture0;
unsigned int texture1;
unsigned int textureGround;
unsigned int textureFrame;
Shader shader;
Shader shaderPostProcess;

// 相机的参数
// 传递给lookAt
float cameraPos[3] = {0.0f, -10.0f, 1.0f};
float cameraFront[3] = {0.0f, 1.0f, 0.0f};
float cameraUp[3] = {0.0f, 0.0f, 1.0f};
// 物体颜色和光照颜色
float objectColor[3] = {1.0f, 0.8f, 0.1f};
float lightColor[3] = {1.0f,1.0f,1.0f};
// 光照的位置
float lightPos[4][3] = 
{ 
    { 2.0f, -1.5f,  1.2f},
    { 2.0f,  2.0f,  2.0f},
};
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

GLuint textureDiffuse;
GLuint textureSpecular;

float cubePosition[5][3] =
{
    { 0.0f,  0.0f,  0.5f},
    { 2.0f,  5.0f,  2.5f},
    {-1.5f, -2.2f,  1.5f},  
    {-3.8f,  2.0f,  0.5f},  
    { 2.4f, -0.4f,  3.5f},  
};

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

    glGenVertexArrays(1,&postVAO);
    glBindVertexArray(postVAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));

    glBindVertexArray(0);
    // 创建纹理并加载纹理
    textureDiffuse = Texture::loadTexture(GL_TEXTURE_2D,"./images/woodbox_diffuse.png");
    textureSpecular = Texture::loadTexture(GL_TEXTURE_2D,"./images/woodbox_specular.png");
	textureGround = Texture::loadTexture(GL_TEXTURE_2D,"./images/ground.png");

    // 初始化Shader
    shader.compile(GL_VERTEX_SHADER,"./default_vert.glsl",
        GL_FRAGMENT_SHADER,"./default_frag.glsl");
    // 初始化后置处理的shader
	shaderPostProcess.compile(GL_VERTEX_SHADER,"./postprocess_vert.glsl",
		GL_FRAGMENT_SHADER,"./postprocess_frag.glsl");

	// 创建一个新的framebuffer
	glGenFramebuffers(1,&FBO);
	glBindFramebuffer(GL_FRAMEBUFFER,FBO);
	/*glBindFramebuffer(GL_READ_FRAMEBUFFER,FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FBO);*/
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		glGenTextures(1,&textureFrame);
		glBindTexture(GL_TEXTURE_2D,textureFrame);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,640,480,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
		//glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,640,480,0,GL_RGB,GL_UNSIGNED_BYTE,NULL); //depth buffer
		//glTexImage2D(GL_TEXTURE_2D,0,GL_STENCIL_INDEX,640,480,0,GL_RGB,GL_UNSIGNED_BYTE,NULL); //stencil buffer
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glBindTexture(GL_TEXTURE_2D,0);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,textureFrame,0);
		//glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,textureFrame,0); //depth buffer
		//glFramebufferTexture2D(GL_FRAMEBUFFER,GL_STENCIL_ATTACHMENT,GL_TEXTURE_2D,textureFrame,0); //stencil buffer

		//如果要将depth buffer和stencil buffer绑定到同一个texture中
		//glTexImage2D(
		//  GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0, 
		//  GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
		//);

		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
	
		// 使用renderbuffer的例子
		// renderbuffer是只写的
		glGenRenderbuffers(1,&RBO);
		glBindRenderbuffer(GL_RENDERBUFFER,RBO);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,800,600);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,RBO);
	}

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		printf("FRAMEBUFFER ERROR: The framebuffer is not completed");
	}
	

	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

// 绘制函数
// 参数:
//     window : GLFW的窗口指针	
void onDraw(GLFWwindow* window){
	glBindFramebuffer(GL_FRAMEBUFFER,FBO);
	glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
    shader.use();

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
    shader.setMat4v("view",viewMat.data(),true);
    // view坐标系到clip坐标系
    mat4 projectMat;
    mat::perspective(projectMat,TO_RADIAN(45),aspect,0.1f,100.0f);
    //mat::ortho(projectMat,-2,2,-2,2,0.1,20);
    shader.setMat4v("project",projectMat.data(),true);
    


    // texture0绑定到texture单元0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureDiffuse);
    //再将texture单元0绑定到shader的sampler2D
    shader.setInt("material.diffuse",0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,textureSpecular);
    shader.setInt("material.specular",1);

    // 向shader传递物体材质参数和光照颜色
    shader.setVec3("material.specular",0.992157f,0.941176f,0.807843f);
    shader.setFloat("material.shininess",0.21794872f * 128.0f);
    // 第一个光照(平行光)
    shader.setVec3v("dirLight.dir",lightPos[0]);
    shader.setVec3("dirLight.ambient",1.0f,1.0f,1.0f);
    shader.setVec3("dirLight.diffuse",1.0f,1.0f,1.0f);
    shader.setVec3("dirLight.specular",1.0f,1.0f,1.0f);
    // 其他三个光照(点光源)
    shader.setVec3v("pointLight.pos",lightPos[1]);
    shader.setFloat("pointLight.kc",1.0f);
    shader.setFloat("pointLight.kl",0.007f);
    shader.setFloat("pointLight.kq",0.0002f);
    shader.setVec3("pointLight.ambient",1.0f,1.0f,1.0f);
    shader.setVec3("pointLight.diffuse",1.0f,1.0f,1.0f);
    shader.setVec3("pointLight.specular",1.0f,1.0f,1.0f);

    // 向shader传递相机位置
    shader.setVec3v("cameraPos",cameraPos);

    glBindVertexArray(VAO);
    for(int i=0; i<5; i++){
        mat4 transform(1.0f);
        mat::translate(transform, cubePosition[i][0], cubePosition[i][1], cubePosition[i][2]);
        shader.setMat4v("transform",transform.data(),true);

        glDrawArrays(GL_TRIANGLES,0,36);
    }
	// 底面纹理
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureGround);
    shader.setInt("material.diffuse",0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,textureGround);
    shader.setInt("material.specular",1);

	//取cube的顶部作为地平面
	mat4 transform(1.0f);
	mat::scale(transform,40.0f,40.0f,0.0f);
	//mat::translate(transform,0.0f,0.0f,-0.5f);
    shader.setMat4v("transform",transform.data(),true);
	glDrawArrays(GL_TRIANGLES,6,6);



	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	shaderPostProcess.use();
	transform = mat4(1.0);
	mat::scale(transform,2.0f,2.0f,0.0f);
	shaderPostProcess.setMat4v("transform",transform.data(),true);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,textureFrame);
	shaderPostProcess.setInt("texturePostProcess",0);
	
	glBindVertexArray(postVAO);
	glDrawArrays(GL_TRIANGLES,6,6);

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
    window = glfwCreateWindow(640, 480, "OpenGL's road - New framebuffer for post processing", NULL, NULL);
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