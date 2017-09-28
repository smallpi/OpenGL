#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(void):
	m_textureId(0)
{
}


Texture::~Texture(void)
{
}

GLuint Texture::loadTexture(GLenum type, const char* file)
{
	stbi_set_flip_vertically_on_load(true); //�Զ��������·�ת

	GLuint texture = 0;
	glGenTextures(1, &texture);

	int width,height,nChannel;
    unsigned char* data = stbi_load(file,&width,&height,&nChannel,0);
    if(data){
		GLenum format;
		switch(nChannel){
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		}
        //��7������ʱͼ�����ɫ��ʽ
        //��3������������Ҫ�洢����ɫ��ʽ
		glBindTexture(GL_TEXTURE_2D,texture);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,format,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);

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
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// ����mipmapʱ��ȡ����ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }else{
        fprintf(stderr,"Fail to load Texture");
    }
    stbi_image_free(data);

	return texture;
}