/**
 *	author: smallpi
 *	date: 2017-08-20		
 *	description: 
 *		��һ�����ν���������ͼ
 *		�����������ɫ���Լ�ָ������ɫ���л��
 *		����һ��������ͼ����ɫ��ϵľ���
 *
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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"

#include <stdio.h>


unsigned int VAO;
unsigned int VBO;
unsigned int texture;
unsigned int EBO;
GLuint shaderId;

// ��ʼ������
void onInit(void){
    //��ʼ��OpenGL��������
    glClearColor(0.2f,0.5f,0.4f,1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
	//����ɫҪָ��ΪGL_SMOOTH(Ĭ��),����ΪGL_FLAT
	glShadeModel(GL_SMOOTH);

    // ��ʼ��GLEW��
    if(glewInit() != GLEW_OK){
        exit(-1);
    }

    //��ʼ������
	//���εĶ������� ��ɫ���� �� ��������
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};
    // VertexArrays�Ĵ���ֻ��Ϊ�˼�����
    glGenVertexArrays(1, &VAO);
    // ����һ��VBO���ڴ洢����Ķ�������
    glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);
	//��VBO�Ķ��㲿������ָ��shader��location=0
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);  // ��ʱ���仯��
	//��VBO����ɫ��������ָ��shader��location=1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
	//��VBO���������겿������ָ��shader��location=2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    glBindVertexArray(0);


	// ����������������
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);

	int width,height,nChannel;
	unsigned char* data = stbi_load("./images/wall.png",&width,&height,&nChannel,0);
	if(data){
		//��7������ʱͼ�����ɫ��ʽ
		//��3������������Ҫ�洢����ɫ��ʽ
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}else{
		fprintf(stderr,"Fail to load Texture");
	}
	stbi_image_free(data);
	// ��ʼ����������
	// �������곬��0~1��Χ����ô����
	//		GL_REPEAT	�ظ�
	//		GL_MIRRORED_REPEAT	�ظ�����
	//		GL_CLAMP_TO_EDGE	�ͱ�Ե��ͬ
	//		GL_CLAMP_TO_BORDER	��ָ���ı�Ե��ɫ��ͬ
	//			ͨ��GL_TEXTURE_BORDER_COLOR�趨
	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// �����ȡ����ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ����mipmapʱ��ȡ����ʽ
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // ��ʼ��Shader
    vector<GLuint> shaderList;
    shaderList.push_back(Shader::createShader(GL_VERTEX_SHADER,"./default_vert.glsl"));
    shaderList.push_back(Shader::createShader(GL_FRAGMENT_SHADER,"./default_frag.glsl"));
    shaderId = Shader::createProgram(shaderList);

}

// ���ƺ���
// ����:
//     window : GLFW�Ĵ���ָ��	
void onDraw(GLFWwindow* window){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderId);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,texture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

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
    glViewport(0,0,width,height);
    onDraw(window);
}

// ����ƶ�������
// ����:
//     window : GLFW�Ĵ���ָ��
//     x : ������ڵ�x����
//     y : ������ڵ�y����
void onMouseMove(GLFWwindow* window, double x, double y){
    
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
    window = glfwCreateWindow(640, 480, "OpenGL's road - Color Mixed Textured rectangle", NULL, NULL);
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

    // �Ѹô������óɵ�ǰ��OpenGL����
    glfwMakeContextCurrent(window);
    // �Լ�д�ĳ�ʼ������
    onInit();

    // ����ѭ��ֱ���رմ���
    while (!glfwWindowShouldClose(window))
    {
        //�Լ�д�Ļ��ƺ���
        onDraw(window);

        //�¼����д���
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}