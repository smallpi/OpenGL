/**
 *	author: smallpi
 *	date: 2017-09-11		
 *	description: 
 *		���Դ
 *		ͨ��shadow mapping�ķ�ʽ��Ⱦ��Ӱ
 *		��Ҫһ��Framebuffer����depthmap
 *		�����ƽ�й⣬���Դ��Ҫ��cubemap����depthmap���ͼ
 *
 *		shadow mapping��Ҫ�����һЩ����
 *		1. ����depthmap�������������������������
 *			��������Զ�depthmap����һ��biasֵ���
 *			biasֵӦ�ù��սǶȶ��仯��Խ��б��biasԽ��
 *		2. ����biasƫ�Ʋ�������Ӱ��������
 *		3. ����depthmap��С��˵ģ�depthmap�ⲿ������Ӱ������
 *		4. ����depthmap�ֱ�������ľ������
 *
 *		���������������
 *			�����ӵķ�����û���趨��ȷ�����¹��պ����
 **/

/**
 *	������ 
 *		GLFW3.2.1 (���ڴ�������)
 *			lib: glfw3.lib
 *			dll: glfw3.dll
 *		GLEW0.96 (����ʹ�ø߰汾��OpenGL API)
 *			lib: glew32.lib
 *			dll: glew32.dll
 *		stb_image
 *			stbimage.h
 **/


//���뵼��glew.h �ٵ���gl.h
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "Shader.h" // ����shader�ĸ����� �ο�learnopengl.com
#include "Texture.h" // �Լ�д��Texture������
#include "mat.h" // �Լ�д��һ�������࣬�ṩ��ƽ����ת���ŵķ���

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

// ����Ĳ���
// ���ݸ�lookAt
float cameraPos[3] = {0.0f, 0.0f, 10.0f};
float cameraFront[3] = {0.0f, 0.0f, -1.0f};
float cameraUp[3] = {0.0f, 1.0f, 0.0f};
// ������ɫ�͹�����ɫ
float objectColor[3] = {1.0f, 0.8f, 0.1f};
float lightColor[3] = {1.0f,1.0f,1.0f};
// ���յ�λ��
float lightPos[3] = 
{ 
    0.0f,  4.0f,  0.0f
};
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// �������λ��
float lastX = 320.0f;
float lastY = 240.0f;
// ŷ��ˮƽ�Ǻ͸�����
float yaw = 0.0f;
float pitch = 0.0f;
bool firstMove = true; //�����һ���ƶ�������������

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

// ��ʼ������
void onInit(void){

    //��ʼ��OpenGL��������
    glClearColor(0.1f,0.5f,0.4f,1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    //����ɫҪָ��ΪGL_SMOOTH(Ĭ��),����ΪGL_FLAT
    glShadeModel(GL_SMOOTH);

    // ��ʼ��GLEW��
    if(glewInit() != GLEW_OK){
        exit(-1);
    }

    //��ʼ������
    //���εĶ�������			��������			����������
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
    // VertexArrays�Ĵ���ֻ��Ϊ�˼�����
    glGenVertexArrays(1, &VAO);
    // ����һ��VBO���ڴ洢����Ķ�������
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);
    //��VBO�Ķ��㲿������ָ��shader��location=0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    //��VBO������������ָ��shader��location=1
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    //��VBO�ķ�������������ָ��shader��location=2
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

    // ����������������
    textureDiffuse = Texture::loadTexture(GL_TEXTURE_2D,"./images/woodbox_diffuse.png");
    textureSpecular = Texture::loadTexture(GL_TEXTURE_2D,"./images/woodbox_specular.png");
	textureWall = Texture::loadTexture(GL_TEXTURE_2D,"./images/wall.png");


	
	// ���ɴ洢depthmap��framebuffer
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



    // ��ʼ��Shader
    shader.compile(GL_VERTEX_SHADER,"./default_vert.glsl",
        GL_FRAGMENT_SHADER,"./default_frag.glsl");
    // ��ʼ����Դ�������shader
    lampShader.compile(GL_VERTEX_SHADER,"./lamp_vert.glsl",
        GL_FRAGMENT_SHADER,"./lamp_frag.glsl");
	depthmapShader.compile(GL_VERTEX_SHADER,"./depthmap_vert.glsl",
		GL_GEOMETRY_SHADER,"./depthmap_geom.glsl",
		GL_FRAGMENT_SHADER,"./depthmap_frag.glsl");
}

// ���ƺ���
// ����:
//     window : GLFW�Ĵ���ָ��	
void onDraw(GLFWwindow* window){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // ����ŷ��ˮƽ�Ǻ͸����Ǽ���cameraFront
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
	//	ע�⣬��ǰ�����ҵ�cubemap�У�ͷ���ϵ�λ�ö����෴�ġ�
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
	
	// ����������
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
	
	// ������
	glBindVertexArray(VAO);
	shader.setInt("isBox",1);
	glDrawArrays(GL_TRIANGLES,0,36);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,textureDiffuse);
	shader.setInt("material.diffuse",0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,textureSpecular);
	shader.setInt("material.specular",1);

	// С����
	shader.setInt("isBox",0);
	for(int i=0; i<5; i++){
		transform = mat4(1.0f);
		mat::translate(transform,cubePosition[i][0],cubePosition[i][1],cubePosition[i][2]);
		shader.setMat4v("transform",transform.data(),true);
		glDrawArrays(GL_TRIANGLES,0,36);
	}

	// ����������
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

// ���̴�����
// ����:
//     window : GLFW�Ĵ���ָ��
//     key : ����ʲô��
void onKey(GLFWwindow* window, int key, int scanCode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window,GLFW_TRUE);
    }
}

// ���ڴ�С�仯������
// ����:
//     window : GLFW�Ĵ���ָ��
//     width : ���ڿ��
//     height : ���ڸ߶�
void onSize(GLFWwindow* window, int width, int height){

	// ������Ļ��Ⱥ͸߶�
	screen_width = width;
	screen_height = height;

    glViewport(0,0,width,height);
    aspect = width / height;

    onDraw(window);
}

// ����ƶ�������
// ����:
//     window : GLFW�Ĵ���ָ��
//     x : ������ڵ�x����
//     y : ������ڵ�y����
void onMouseMove(GLFWwindow* window, double x, double y){
    
    //�����һ���ƶ�������������
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

// ������������
void onMouseWheel(GLFWwindow* window, double xoffset, double yoffset){
    
}

// ��갴��������

void onMouseButton(GLFWwindow* window, int button, int action, int mods){
    switch(button){
    case GLFW_MOUSE_BUTTON_LEFT:	
        {
            //����������
        }
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        {
            //����м�����
        }
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        {
            //����Ҽ�����
        }
        break;
    }
}

// ����ס�����¼�
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

// ��������
//     ����
//     error : �������
//     description: ����˵��
void onError( int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
}


// main���� ��������
int main(int argc, char** argv){
    GLFWwindow* window;

    // ��ʼ��GLFW��
    if (!glfwInit()){
        return -1;
    }


    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //������ߵ�OpenGL�汾
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0); //������͵�OpenGL�汾
    // ����windows mode�Ĵ���
    // ������OpenGL����
    window = glfwCreateWindow(screen_width, screen_height, "OpenGL's road - Depth Field", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetErrorCallback(onError); //���ô�������
    glfwSetKeyCallback(window,onKey); //���ü��̴�����
    glfwSetFramebufferSizeCallback(window,onSize); //���ô��ڴ�С�仯������
    glfwSetCursorPosCallback(window,onMouseMove); //��������ƶ�������
    glfwSetScrollCallback(window,onMouseWheel); //���������ִ�����
    glfwSetMouseButtonCallback(window,onMouseButton); //������갸������

    //������꣬�����Բ�������¼�
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // �Ѹô������óɵ�ǰ��OpenGL����
    glfwMakeContextCurrent(window);
    // �Լ�д�ĳ�ʼ������
    onInit();

    // ����ѭ��ֱ���رմ���
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processKeyInput(window);

        //�Լ�д�Ļ��ƺ���
        onDraw(window);

        //�¼����д���
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}