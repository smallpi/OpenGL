/**
 *	author: smallpi
 *	date: 2017-09-11		
 *	description: 
 *		点光源
 *		通过shadow mapping的方式渲染阴影
 *		需要一个Framebuffer保存depthmap
 *		相比于平行光，点光源需要用cubemap保存depthmap深度图
 *
 *		shadow mapping需要解决的一些问题
 *		1. 由于depthmap采用引起的误差产生的条纹问题
 *			该问题可以对depthmap减少一个bias值解决
 *			bias值应该光照角度而变化，越倾斜，bias越大
 *		2. 由于bias偏移产生的阴影脱离问题
 *		3. 由于depthmap大小因此的，depthmap外部处于阴影的问题
 *		4. 由于depthmap分辨率引起的锯齿问题
 *
 *		编程中遇到的问题
 *			大箱子的法向量没有设定正确，导致光照很奇怪
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

const unsigned int SHADOW_WIDTH = 1024;
const unsigned int SHADOW_HEIGHT = 1024;
unsigned int screen_width = 1280;
unsigned int screen_height = 720;

unsigned int depthmapFBO;
unsigned int depthmapTexture;

unsigned int VAO;
unsigned int lampVAO;
unsigned int VBO;
Shader shader;
Shader lampShader;
Shader depthmapShader;

// 相机的参数
// 传递给lookAt
float cameraPos[3] = {0.0f, 0.0f, 10.0f};
float cameraFront[3] = {0.0f, 0.0f, -1.0f};
float cameraUp[3] = {0.0f, 1.0f, 0.0f};
// 物体颜色和光照颜色
float objectColor[3] = {1.0f, 0.8f, 0.1f};
float lightColor[3] = {1.0f,1.0f,1.0f};
// 光照的位置
float lightPos[3] = 
{ 
    0.0f,  4.0f,  0.0f
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

float aspect = screen_width / (float)screen_height;

GLuint textureDiffuse;
GLuint textureSpecular;
unsigned int textureWall;

float cubePosition[5][3] = { 
	{4.0f, 4.0f, 4.0f},
	{-2.0f, 2.0f, -5.0f },
	{1.0f, 6.0f, 3.0f},
	{4.0f, 2.0f, 7.0f},
	{0.0f, 1.0f, -4.0f}
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
    //矩形的顶点数据			纹理坐标			法向量坐标
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

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
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
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
    textureDiffuse = Texture::loadTexture(GL_TEXTURE_2D,"./images/woodbox_diffuse.png");
    textureSpecular = Texture::loadTexture(GL_TEXTURE_2D,"./images/woodbox_specular.png");
	textureWall = Texture::loadTexture(GL_TEXTURE_2D,"./images/wall.png");


	
	// 生成存储depthmap的framebuffer
	glGenFramebuffers(1, &depthmapFBO);
	
	glGenTextures(1, &depthmapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthmapTexture);
	for(int i=0; i<6; i++){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_DEPTH_COMPONENT,
			SHADOW_WIDTH,SHADOW_HEIGHT,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, depthmapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,depthmapTexture,0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER,0);



    // 初始化Shader
    shader.compile(GL_VERTEX_SHADER,"./default_vert.glsl",
        GL_FRAGMENT_SHADER,"./default_frag.glsl");
    // 初始化光源正方体的shader
    lampShader.compile(GL_VERTEX_SHADER,"./lamp_vert.glsl",
        GL_FRAGMENT_SHADER,"./lamp_frag.glsl");
	depthmapShader.compile(GL_VERTEX_SHADER,"./depthmap_vert.glsl",
		GL_GEOMETRY_SHADER,"./depthmap_geom.glsl",
		GL_FRAGMENT_SHADER,"./depthmap_frag.glsl");
}

// 绘制函数
// 参数:
//     window : GLFW的窗口指针	
void onDraw(GLFWwindow* window){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // 根据欧拉水平角和俯仰角计算cameraFront
    cameraFront[0] = - cos(TO_RADIAN(pitch)) * sin(TO_RADIAN(yaw));
	cameraFront[1] = sin(TO_RADIAN(pitch));
	cameraFront[2] = - cos(TO_RADIAN(pitch)) * cos(TO_RADIAN(yaw));


	mat4 projectMat;
    mat4 viewMat;
	mat::perspective(projectMat,TO_RADIAN(45),aspect,0.1f,100.0f);
    mat::lookAt(viewMat,
        cameraPos[0],cameraPos[1],cameraPos[2],
        cameraPos[0]+cameraFront[0],cameraPos[1]+cameraFront[1],cameraPos[2]+cameraFront[2],
        cameraUp[0],cameraUp[1],cameraUp[2]);
	depthmapShader.use();
	mat4 projectMatDepthmap;
	mat4 viewMatDepthmap[6];
	float near = 1.0f;
	float far = 25.0f;
	//
	//	注意，在前后左右的cubemap中，头朝上的位置都是相反的。
	//
	mat::perspective(projectMatDepthmap,TO_RADIAN(90),1,near,far);
	mat::lookAt(viewMatDepthmap[0],lightPos[0],lightPos[1],lightPos[2],
		lightPos[0] + 1.0f,lightPos[1],lightPos[2],
		0.0f,-1.0f,0.0f);		
	mat::lookAt(viewMatDepthmap[1],lightPos[0],lightPos[1],lightPos[2],
		lightPos[0] - 1.0f,lightPos[1],lightPos[2],
		0.0f,-1.0f,0.0f);
	mat::lookAt(viewMatDepthmap[2],lightPos[0],lightPos[1],lightPos[2],
		lightPos[0],lightPos[1] + 1.0f,lightPos[2],
		0.0f,0.0f,1.0f);
	mat::lookAt(viewMatDepthmap[3],lightPos[0],lightPos[1],lightPos[2],
		lightPos[0],lightPos[1] - 1.0f,lightPos[2],
		0.0f,0.0f,-1.0f);
	mat::lookAt(viewMatDepthmap[4],lightPos[0],lightPos[1],lightPos[2],
		lightPos[0],lightPos[1],lightPos[2]+1.0f,
		0.0f,-1.0f,0.0f);
	mat::lookAt(viewMatDepthmap[5],lightPos[0],lightPos[1],lightPos[2],
		lightPos[0],lightPos[1],lightPos[2]-1.0f,
		0.0f,-1.0f,0.0f);
	depthmapShader.setMat4v("project",projectMatDepthmap.data(),true);
	depthmapShader.setMat4v("view[0]",viewMatDepthmap[0].data(),true);
	depthmapShader.setMat4v("view[1]",viewMatDepthmap[1].data(),true);
	depthmapShader.setMat4v("view[2]",viewMatDepthmap[2].data(),true);
	depthmapShader.setMat4v("view[3]",viewMatDepthmap[3].data(),true);
	depthmapShader.setMat4v("view[4]",viewMatDepthmap[4].data(),true);
	depthmapShader.setMat4v("view[5]",viewMatDepthmap[5].data(),true);


	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	mat4 transform(1.0f);
	mat::scale(transform,20.0f,20.0f,20.0f);
	depthmapShader.setMat4v("transform",transform.data(),true);

	depthmapShader.setFloat("farPlane",far);
	depthmapShader.setVec3v("lightPos",lightPos);

	glBindFramebuffer(GL_FRAMEBUFFER,depthmapFBO);
	glViewport(0,0,SHADOW_WIDTH,SHADOW_HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES,0,36);
	for(int i=0; i<5; i++){
		transform = mat4(1.0f);
		mat::translate(transform,cubePosition[i][0],cubePosition[i][1],cubePosition[i][2]);
		depthmapShader.setMat4v("transform",transform.data(),true);
		glDrawArrays(GL_TRIANGLES,0,36);
	}

	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	
	// 真正的物体
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glViewport(0,0,screen_width,screen_height);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	
	shader.use();
	shader.setMat4v("project",projectMat.data(),true);
	shader.setMat4v("view",viewMat.data(),true);
	transform = mat4(1.0f);
	mat::scale(transform,20.0f,20.0f,20.0f);
	shader.setMat4v("transform",transform.data(),true);

	shader.setFloat("farPlane",far);
	shader.setVec3v("cameraPos",cameraPos);
	shader.setFloat("material.shininess",32);

	shader.setVec3v("pointLight.pos",lightPos);
	shader.setFloat("pointLight.kc",1.0f);
	shader.setFloat("pointLight.kl",0.00001f);
	shader.setFloat("pointLight.kq",0.00000001f);
	shader.setVec3("pointLight.ambient",1.0f,1.0f,1.0f);
	shader.setVec3("pointLight.diffuse",1.0f,1.0f,1.0f);
	shader.setVec3("pointLight.specular",1.0f,1.0f,1.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,textureWall);
	shader.setInt("material.diffuse",0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,textureWall);
	shader.setInt("material.specular",1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP,depthmapTexture);
	shader.setInt("cubeDepthmap",2);
	
	// 大箱子
	glBindVertexArray(VAO);
	shader.setInt("isBox",1);
	glDrawArrays(GL_TRIANGLES,0,36);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,textureDiffuse);
	shader.setInt("material.diffuse",0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,textureSpecular);
	shader.setInt("material.specular",1);

	// 小箱子
	shader.setInt("isBox",0);
	for(int i=0; i<5; i++){
		transform = mat4(1.0f);
		mat::translate(transform,cubePosition[i][0],cubePosition[i][1],cubePosition[i][2]);
		shader.setMat4v("transform",transform.data(),true);
		glDrawArrays(GL_TRIANGLES,0,36);
	}

	// 代表光的物体
	lampShader.use();
	lampShader.setMat4v("project",projectMat.data(),true);
	lampShader.setMat4v("view",viewMat.data(),true);

	transform = mat4(1.0f);
	mat::translate(transform,lightPos[0],lightPos[1],lightPos[2]);
	lampShader.setMat4v("transform",transform.data(),true);


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
}

// 窗口大小变化处理函数
// 参数:
//     window : GLFW的窗口指针
//     width : 窗口宽度
//     height : 窗口高度
void onSize(GLFWwindow* window, int width, int height){

	// 保存屏幕宽度和高度
	screen_width = width;
	screen_height = height;

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
    window = glfwCreateWindow(screen_width, screen_height, "OpenGL's road - Depth Field", NULL, NULL);
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