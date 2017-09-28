
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GL/glew.h>

class Texture
{
public:
	Texture(void);
	~Texture(void);

	static GLuint loadTexture(GLenum type, const char* file);

private:

	GLuint m_textureId;
};

#endif