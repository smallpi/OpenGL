
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GL/glew.h>
#include <vector>
#include <string>
using namespace std;

class Texture
{
public:
	Texture(void);
	~Texture(void);

	static GLuint loadTexture(GLenum type, const char* file);
	static GLuint loadCubemap(vector<string>& faces);

private:

	GLuint m_textureId;
};

#endif