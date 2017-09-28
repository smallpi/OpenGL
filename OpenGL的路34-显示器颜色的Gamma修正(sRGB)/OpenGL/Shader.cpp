#include "Shader.h"

#include <fstream>
#include <sstream>

Shader::Shader(void):
	m_shaderId(0)
{
}


Shader::~Shader(void)
{
	if(m_shaderId != SHADER_DEFAULT_ID){
		glDeleteProgram(m_shaderId);
	}
}

Shader::Shader(GLenum type0, const char* file0,
	GLenum type1, const char* file1):
	m_shaderId(0)
{
	compile(type0,file0,type1,file1);
}

Shader::Shader(GLenum type0, const char* file0,
	GLenum type1, const char* file1,
	GLenum type2, const char* file2):
	m_shaderId(0)
{
	compile(type0,file0,type1,file1,type2,file2);
}

Shader::Shader(GLenum type0, const char* file0,
	GLenum type1, const char* file1,
	GLenum type2, const char* file2,
	GLenum type3, const char* file3):
	m_shaderId(0)
{
	compile(type0,file0,type1,file1,type2,file2,type3,file3);
}

Shader::Shader(GLenum type0, const char* file0,
	GLenum type1, const char* file1,
	GLenum type2, const char* file2,
	GLenum type3, const char* file3,
	GLenum type4, const char* file4):
	m_shaderId(0)
{
	compile(type0,file0,type1,file1,type2,file2,type3,file3,type4,file4);
}

void Shader::compile(GLenum type0, const char* file0,
	GLenum type1, const char* file1)
{
	if(m_shaderId != SHADER_DEFAULT_ID){
		glDeleteProgram(m_shaderId);
	}
	vector<GLuint> shaderList;
	shaderList.push_back(Shader::createShader(type0,file0));
	shaderList.push_back(Shader::createShader(type1,file1));
	m_shaderId = Shader::createProgram(shaderList);
}

void Shader::compile(GLenum type0, const char* file0,
	GLenum type1, const char* file1,
	GLenum type2, const char* file2)
{
	if(m_shaderId != SHADER_DEFAULT_ID){
		glDeleteProgram(m_shaderId);
	}
	vector<GLuint> shaderList;
	shaderList.push_back(Shader::createShader(type0,file0));
	shaderList.push_back(Shader::createShader(type1,file1));
	shaderList.push_back(Shader::createShader(type2,file2));
	m_shaderId = Shader::createProgram(shaderList);
}

void Shader::compile(GLenum type0, const char* file0,
	GLenum type1, const char* file1,
	GLenum type2, const char* file2,
	GLenum type3, const char* file3)
{
	if(m_shaderId != SHADER_DEFAULT_ID){
		glDeleteProgram(m_shaderId);
	}
	vector<GLuint> shaderList;
	shaderList.push_back(Shader::createShader(type0,file0));
	shaderList.push_back(Shader::createShader(type1,file1));
	shaderList.push_back(Shader::createShader(type2,file2));
	shaderList.push_back(Shader::createShader(type3,file3));
	m_shaderId = Shader::createProgram(shaderList);
}

void Shader::compile(GLenum type0, const char* file0,
	GLenum type1, const char* file1,
	GLenum type2, const char* file2,
	GLenum type3, const char* file3,
	GLenum type4, const char* file4)
{
	if(m_shaderId != SHADER_DEFAULT_ID){
		glDeleteProgram(m_shaderId);
	}
	vector<GLuint> shaderList;
	shaderList.push_back(Shader::createShader(type0,file0));
	shaderList.push_back(Shader::createShader(type1,file1));
	shaderList.push_back(Shader::createShader(type2,file2));
	shaderList.push_back(Shader::createShader(type3,file3));
	shaderList.push_back(Shader::createShader(type4,file4));
	m_shaderId = Shader::createProgram(shaderList);
}


//	编译单个shader
//	参数:
//		eShaderType : shader的类型
//		strShaderFile : shader内容字符串
GLuint Shader::createShader(GLenum eShaderType, const string &strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	const char* strFileData = strShaderFile.c_str();
	glShaderSource(shader,1,&strFileData,NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&status);
	if( status == GL_FALSE )
	{
		GLint infoLogLength;
		glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength+1];
		glGetShaderInfoLog(shader,infoLogLength,NULL,strInfoLog);
		const char* strShaderType = NULL;
		switch(eShaderType)
		{
			case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
			case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
			case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}
		fprintf(stderr,"Compile failure in %s shader:\n%s\n",strShaderType,strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}

//	编译shader文件
//	参数:
//		eShaderType: shader的类型
//		fileName: shader的源文件
GLuint Shader::createShader(GLenum eShaderType, const char* fileName)
{
	ifstream infile(fileName);
	if(!infile)
	{
		fprintf(stderr,"Could not open file: %s for reading.", fileName);
		return 0;
	}
	stringstream buffer;
	buffer << infile.rdbuf();
	infile.close();

	return Shader::createShader(eShaderType,buffer.str());
}

//	根据shader的集合创建shader程序
//	参数:
//		shaderList : shader的集合
GLuint Shader::createProgram(const vector<GLuint> &shaderList)
{
	GLuint programID = glCreateProgram();
	for(vector<GLuint>::size_type iLoop=0; iLoop<shaderList.size(); iLoop++)
	{
		glAttachShader(programID,shaderList[iLoop]);
	}
	glLinkProgram(programID);

	GLint status;
	glGetProgramiv(programID,GL_LINK_STATUS,&status);
	if( status == GL_FALSE )
	{
		GLint infoLogLength;
		glGetProgramiv(programID,GL_INFO_LOG_LENGTH,&infoLogLength);

		GLchar* strInfoLog = new GLchar[infoLogLength+1];
		glGetProgramInfoLog(programID,infoLogLength,NULL,strInfoLog);
		fprintf(stderr,"Linker failure: %s\n",strInfoLog);
		delete[] strInfoLog;
	}

	for(size_t iLoop=0; iLoop<shaderList.size(); iLoop++)
	{
		glDetachShader(programID,shaderList[iLoop]);
	}

	return programID;
}

// 获取shader的ID
GLuint Shader::id(void)
{
	return m_shaderId;
}

// glUseProgram
void Shader::use(void)
{
	glUseProgram(m_shaderId);
}

// 向shader传递uniform数据
void Shader::setInt(const char* name, int value){
	glUniform1i(glGetUniformLocation(m_shaderId,name),value);
}

void Shader::setFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(m_shaderId,name),value);
}

void Shader::setDouble(const char* name, double value)
{
	glUniform1d(glGetUniformLocation(m_shaderId,name),value);
}

void Shader::setVec3(const char* name, float value0, float value1, float value2)
{
	glUniform3f(glGetUniformLocation(m_shaderId,name),value0,value1,value2);
}

void Shader::setVec3v(const char* name, float* pValue)
{
	glUniform3fv(glGetUniformLocation(m_shaderId,name),1,pValue);
}

void Shader::setDvec3(const char* name, double value0, double value1, double value2)
{
	glUniform3d(glGetUniformLocation(m_shaderId,name),value0,value1,value2);
}

void Shader::setDvec3v(const char* name, double* pValue)
{
	glUniform3dv(glGetUniformLocation(m_shaderId,name),1,pValue);
}

void Shader::setMat3v(const char* name, float* pValue, bool transpose)
{
	glUniformMatrix3fv(glGetUniformLocation(m_shaderId,name),1,transpose,pValue);
}

void Shader::setDmat3v(const char* name, double* pValue, bool transpose)
{
	glUniformMatrix3dv(glGetUniformLocation(m_shaderId,name),1,transpose,pValue);
}

void Shader::setMat4v(const char* name, float* pValue, bool transpose)
{
	glUniformMatrix4fv(glGetUniformLocation(m_shaderId,name),1,transpose,pValue);
}

void Shader::setDmat4v(const char* name, double* pValue, bool transpose)
{
	glUniformMatrix4dv(glGetUniformLocation(m_shaderId,name),1,transpose,pValue);
}