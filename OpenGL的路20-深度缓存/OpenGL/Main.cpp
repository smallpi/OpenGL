/**
 *	author: smallpi
 *	date: 2017-08-24		
 *	description: 
 *		�������Ե����ֵ��������Ե����ֵ
 *		��������ȳ�
 *		����Ӧ���� ����ĳ���
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


#include "Shader.h" // ����shader�ĸ����� �ο�learnopengl.com
#include "Texture.h" // �Լ�д��Texture������
#include "mat.h" // �Լ�д��һ�������࣬�ṩ��ƽ����ת���ŵķ���

#include <stdio.h>


unsigned int VAO;
unsigned int lampVAO;
unsigned int VBO;
unsigned int texture0;
unsigned int texture1;
unsigned int textureGround;
Shader shader;
Shader lampShader;

// ����Ĳ���
// ���ݸ�lookAt
float cameraPos[3] = {0.0f, -10.0f, 1.0f};
float cameraFront[3] = {0.0f, 1.0f, 0.0f};
float cameraUp[3] = {0.0f, 0.0f, 1.0f};
// ������ɫ�͹�����ɫ
float objectColor[3] = {1.0f, 0.8f, 0.1f};
float lightColor[3] = {1.0f,1.0f,1.0f};
// ���յ�λ��
float lightPos[4][3] = 
{ 
    { 2.0f, -1.5f,  1.2f},
    { 2.0f,  2.0f,  2.0f},
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
    textureDiffuse = Texture::loadTexture(GL_TEXTURE_2D,"./images/woodbox_diffuse.png");
    textureSpecular = Texture::loadTexture(GL_TEXTURE_2D,"./images/woodbox_specular.png");
	textureGround = Texture::loadTexture(GL_TEXTURE_2D,"./images/ground.png");

    // ��ʼ��Shader
    shader.compile(GL_VERTEX_SHADER,"./default_vert.glsl",
        GL_FRAGMENT_SHADER,"./default_frag.glsl");
    // ��ʼ����Դ�������shader
    lampShader.compile(GL_VERTEX_SHADER,"./lamp_vert.glsl",
        GL_FRAGMENT_SHADER,"./lamp_frag.glsl");
}

// ���ƺ���
// ����:
//     window : GLFW�Ĵ���ָ��	
void onDraw(GLFWwindow* window){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	
    shader.use();


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
    shader.setMat4v("view",viewMat.data(),true);
    // view����ϵ��clip����ϵ
    mat4 projectMat;
    mat::perspective(projectMat,TO_RADIAN(45),aspect,0.1f,100.0f);
    //mat::ortho(projectMat,-2,2,-2,2,0.1,20);
    shader.setMat4v("project",projectMat.data(),true);
    


    // texture0�󶨵�texture��Ԫ0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureDiffuse);
    //�ٽ�texture��Ԫ0�󶨵�shader��sampler2D
    shader.setInt("material.diffuse",0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,textureSpecular);
    shader.setInt("material.specular",1);

    // ��shader����������ʲ����͹�����ɫ
    shader.setVec3("material.specular",0.992157f,0.941176f,0.807843f);
    shader.setFloat("material.shininess",0.21794872f * 128.0f);
    // ��һ������(ƽ�й�)
    shader.setVec3v("dirLight.dir",lightPos[0]);
    shader.setVec3("dirLight.ambient",1.0f,1.0f,1.0f);
    shader.setVec3("dirLight.diffuse",1.0f,1.0f,1.0f);
    shader.setVec3("dirLight.specular",1.0f,1.0f,1.0f);
    // ������������(���Դ)
    shader.setVec3v("pointLight.pos",lightPos[1]);
    shader.setFloat("pointLight.kc",1.0f);
    shader.setFloat("pointLight.kl",0.007f);
    shader.setFloat("pointLight.kq",0.0002f);
    shader.setVec3("pointLight.ambient",1.0f,1.0f,1.0f);
    shader.setVec3("pointLight.diffuse",1.0f,1.0f,1.0f);
    shader.setVec3("pointLight.specular",1.0f,1.0f,1.0f);

    // ��shader�������λ��
    shader.setVec3v("cameraPos",cameraPos);

    glBindVertexArray(VAO);
    for(int i=0; i<5; i++){
        mat4 transform(1.0f);
        mat::translate(transform, cubePosition[i][0], cubePosition[i][1], cubePosition[i][2]);
        shader.setMat4v("transform",transform.data(),true);

        glDrawArrays(GL_TRIANGLES,0,36);
    }
	// ��������
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureGround);
    shader.setInt("material.diffuse",0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,textureGround);
    shader.setInt("material.specular",1);

	//ȡcube�Ķ�����Ϊ��ƽ��
	mat4 transform(1.0f);
	mat::scale(transform,40.0f,40.0f,0.0f);
	//mat::translate(transform,0.0f,0.0f,-0.5f);
    shader.setMat4v("transform",transform.data(),true);
	glDrawArrays(GL_TRIANGLES,6,6);

    // ���ƹ�Դ������
    lampShader.use();
    
    lampShader.setMat4v("project",projectMat.data(),true);
    lampShader.setMat4v("view",viewMat.data(),true);

    glBindVertexArray(lampVAO);
    for(int i=0; i<2; i++){
        mat4 transform = mat4(1.0f);
        mat::scale(transform,0.2f,0.2f,0.2f);
        mat::translate(transform,lightPos[i][0],lightPos[i][1],lightPos[i][2]);
        lampShader.setMat4v("transform",transform.data(),true);
        glDrawArrays(GL_TRIANGLES,0,36);
    }

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
    window = glfwCreateWindow(640, 480, "OpenGL's road - Depth Field", NULL, NULL);
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