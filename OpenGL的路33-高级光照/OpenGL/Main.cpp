/**
 *	author: smallpi
 *	date: 2017-08-20		
 *	description: 
 *		ͨ��Blinn-Phone Shading����Phone-Shading������specular�ĽǶȿ��ܴ���90�ȵ�����
 *		Blinn-Phone Shading�������ߺ͹��ߵ��м���������ƬԪ�ķ��������бȽ�
 *		��Phone-Shading�е�Specularͨ�����ߵķ��������͹����������бȽ�
 *		ͨ������B��P���л�Blinn-Phone shading��Phone shading
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


#include "Shader.h" // ����shader�ĸ����� �ο�learnopengl.com
#include "mat.h" // �Լ�д��һ�������࣬�ṩ��ƽ����ת���ŵķ���

#include <stdio.h>


unsigned int VAO;
unsigned int lampVAO;
unsigned int VBO;
unsigned int texture0;
unsigned int texture1;
GLuint shaderId;
GLuint lampShaderId;

// ����Ĳ���
// ���ݸ�lookAt
float cameraPos[3] = {0.0f, -10.0f, 0.0f};
float cameraFront[3] = {0.0f, 1.0f, 0.0f};
float cameraUp[3] = {0.0f, 0.0f, 1.0f};
// ������ɫ�͹�����ɫ
float objectColor[3] = {1.0f, 0.8f, 0.1f};
float lightColor[3] = {1.0f,1.0f,1.0f};
// ���յ�λ��
float lightPos[3] = {2.0f,-1.5f,1.2f};
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// �������λ��
float lastX = 320.0f;
float lastY = 240.0f;
// ŷ��ˮƽ�Ǻ͸�����
float yaw = 0.0f;
float pitch = 0.0f;
bool firstMove = true; //�����һ���ƶ�������������

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
    //���εĶ������� �� ��������
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
    //glGenTextures(1,&texture0);
    //glBindTexture(GL_TEXTURE_2D,texture0);

    //int width,height,nChannel;
    //stbi_set_flip_vertically_on_load(true); //�Զ��������·�ת
    //unsigned char* data = stbi_load("./images/wall.png",&width,&height,&nChannel,0);
    //if(data){
    //    //��7������ʱͼ�����ɫ��ʽ
    //    //��3������������Ҫ�洢����ɫ��ʽ
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

    // ��ʼ����������
    // �������곬��0~1��Χ����ô����
    //		GL_REPEAT	�ظ�
    //		GL_MIRRORED_REPEAT	�ظ�����
    //		GL_CLAMP_TO_EDGE	�ͱ�Ե��ͬ
    //		GL_CLAMP_TO_BORDER	��ָ���ı�Ե��ɫ��ͬ
    //			ͨ��GL_TEXTURE_BORDER_COLOR�趨
    //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // �����ȡ����ʽ
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // ����mipmapʱ��ȡ����ʽ
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // ��ʼ��Shader
    vector<GLuint> shaderList;
    shaderList.push_back(Shader::createShader(GL_VERTEX_SHADER,"./default_vert.glsl"));
    shaderList.push_back(Shader::createShader(GL_FRAGMENT_SHADER,"./default_frag.glsl"));
    shaderId = Shader::createProgram(shaderList);
    // ��ʼ����Դ�������shader
    vector<GLuint> lightShaderList;
    lightShaderList.push_back(Shader::createShader(GL_VERTEX_SHADER,"./lamp_vert.glsl"));
    lightShaderList.push_back(Shader::createShader(GL_FRAGMENT_SHADER,"./lamp_frag.glsl"));
    lampShaderId = Shader::createProgram(lightShaderList);
}

// ���ƺ���
// ����:
//     window : GLFW�Ĵ���ָ��	
void onDraw(GLFWwindow* window){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderId);


    // ����ŷ��ˮƽ�Ǻ͸����Ǽ���cameraFront
    cameraFront[0] = cos(TO_RADIAN(pitch)) * cos(TO_RADIAN(yaw+90.0f));
    cameraFront[1] = cos(TO_RADIAN(pitch)) * sin(TO_RADIAN(yaw+90.0f));
    cameraFront[2] = sin(TO_RADIAN(pitch));

    // ��shader���ݱ任����
    // world��ϵת����view����ϵ
    mat4 viewMat;
    mat::lookAt(viewMat,
        cameraPos[0],cameraPos[1],cameraPos[2],
        cameraPos[0]+cameraFront[0],cameraPos[1]+cameraFront[1],cameraPos[2]+cameraFront[2],
        cameraUp[0],cameraUp[1],cameraUp[2]);
    glUniformMatrix4fv(glGetUniformLocation(shaderId,"view"),1,true,viewMat.data());
    // view����ϵ��clip����ϵ
    mat4 projectMat;
    mat::perspective(projectMat,TO_RADIAN(45),aspect,0.1f,100.0f);
    //mat::ortho(projectMat,-2,2,-2,2,0.1,20);
    glUniformMatrix4fv(glGetUniformLocation(shaderId,"project"),1,true,projectMat.data());

    // texture0�󶨵�texture��Ԫ0
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D,texture0);
    // texture1�󶨵�texture��Ԫ1
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D,texture1);
    //�ٽ�texture��Ԫ0�͵�Ԫ1�ֱ�󶨵�shader��sampler2D
    //glUniform1i(glGetUniformLocation(shaderId,"Texture0"),0);
    //glUniform1i(glGetUniformLocation(shaderId,"Texture1"),1);

    mat4 transform(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderId,"transform"),1,true,transform.data());

    // ��shader���ݹ�����ɫ��������ɫ
    glUniform3fv(glGetUniformLocation(shaderId,"objectColor"),1,objectColor);
    glUniform3fv(glGetUniformLocation(shaderId,"lightColor"),1,lightColor);
    // ��shader���ݹ�Դλ�ú����λ��
    glUniform3fv(glGetUniformLocation(shaderId,"lightPos"),1,lightPos);
    glUniform3fv(glGetUniformLocation(shaderId,"cameraPos"),1,cameraPos);
	glUniform1i(glGetUniformLocation(shaderId,"blinnPhone"),blinnPhone);


    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,36);
    

    // ���ƹ�Դ������
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

// ���̴�����
// ����:
//     window : GLFW�Ĵ���ָ��
//     key : ����ʲô��
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

// ���ڴ�С�仯������
// ����:
//     window : GLFW�Ĵ���ָ��
//     width : ���ڿ��
//     height : ���ڸ߶�
void onSize(GLFWwindow* window, int width, int height){
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
    window = glfwCreateWindow(640, 480, "OpenGL's road -Blinn-Phone shading and Phone shading", NULL, NULL);
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