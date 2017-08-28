
#ifndef _SHADER_H_
#define _SHADER_H_

#ifndef SHADER_DEFAULT_ID
#define SHADER_DEFAULT_ID	0
#endif

#ifndef MAT_TRANSPOSE
#define MAT_TRANSPOSE	1
#endif

#include <GL/glew.h>

#include <vector>
#include <string>

using namespace std;

class Shader
{
public:
	Shader(void);
	Shader(GLenum type0, const char* file0,
		GLenum type1, const char* file1);
	Shader(GLenum type0, const char* file0,
		GLenum type1, const char* file1,
		GLenum type2, const char* file2);
	Shader(GLenum type0, const char* file0,
		GLenum type1, const char* file1,
		GLenum type2, const char* file2,
		GLenum type3, const char* file3);
	Shader(GLenum type0, const char* file0,
		GLenum type1, const char* file1,
		GLenum type2, const char* file2,
		GLenum type3, const char* file3,
		GLenum type4, const char* file4);
	~Shader(void);

	static GLuint createShader(GLenum eShaderType, const std::string &strShaderFile);
	static GLuint createShader(GLenum eShaderType, const char* fileName);
	static GLuint createProgram(const vector<GLuint> &shaderList);

	// 获取shader的ID
	GLuint id(void);

	void use(void);

	// 将shader源程序文件编译成shader程序的ID
	void compile(GLenum type0, const char* file0,
		GLenum type1, const char* file1);
	void compile(GLenum type0, const char* file0,
		GLenum type1, const char* file1,
		GLenum type2, const char* file2);
	void compile(GLenum type0, const char* file0,
		GLenum type1, const char* file1,
		GLenum type2, const char* file2,
		GLenum type3, const char* file3);
	void compile(GLenum type0, const char* file0,
		GLenum type1, const char* file1,
		GLenum type2, const char* file2,
		GLenum type3, const char* file3,
		GLenum type4, const char* file4);

	// 向shader传递uniform数据
	void setFloat(const char* name, float value);
	void setDouble(const char* name, double value);
	void setVec3(const char* name, float value0, float value1, float value2);
	void setVec3v(const char* name, float* pValue);
	void setDvec3(const char* name, double value0, double value1, double value2);
	void setDvec3v(const char* name, double* pValue);
	void setMat3v(const char* name, float* pValue, bool transpose=false);
	void setDmat3v(const char* name, double* pValue, bool transpose=false);
	void setMat4v(const char* name, float* pValue, bool transpose=false);
	void setDmat4v(const char* name, double* pValue, bool transpose=false);

protected:

private:
	GLuint m_shaderId;
};

#endif