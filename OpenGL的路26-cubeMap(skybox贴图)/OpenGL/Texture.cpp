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
	stbi_set_flip_vertically_on_load(true); //自动像素上下翻转

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
        //第7个参数时图像的颜色格式
        //第3个参数才是需要存储的颜色格式
		glBindTexture(GL_TEXTURE_2D,texture);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,format,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);

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
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// 设置mipmap时的取样方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }else{
        fprintf(stderr,"Fail to load Texture");
    }
    stbi_image_free(data);

	return texture;
}

GLuint Texture::loadCubemap(vector<string>& faces){
	stbi_set_flip_vertically_on_load(true);

	unsigned int textureId;
	glGenTextures(1,&textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP,textureId);

	int width, height, nChannel;
	for(unsigned i=0; i<faces.size(); i++){
		unsigned char* data = stbi_load(faces[i].c_str(),&width,&height,&nChannel,0);
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

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB,width,height,0,format,GL_UNSIGNED_BYTE,data);
			stbi_image_free(data);
		}else{
			fprintf(stderr,"Fail to load Cubemap Texture");
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureId;
}