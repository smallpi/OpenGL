
#ifndef _SHADER_H_
#define _SHADER_H_

#include <GL/glew.h>

#include <vector>
#include <string>

using namespace std;

class Shader
{
public:
	Shader(void);
	~Shader(void);

	static GLuint createShader(GLenum eShaderType, const std::string &strShaderFile);
	static GLuint createShader(GLenum eShaderType, const char* fileName);
	static GLuint createProgram(const vector<GLuint> &shaderList);
};

#endif