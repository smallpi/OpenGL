#include "Shader.h"

#include <fstream>
#include <sstream>

Shader::Shader(void)
{
}


Shader::~Shader(void)
{
}


//	���뵥��shader
//	����:
//		eShaderType : shader������
//		strShaderFile : shader�����ַ���
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

//	����shader�ļ�
//	����:
//		eShaderType: shader������
//		fileName: shader��Դ�ļ�
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

//	����shader�ļ��ϴ���shader����
//	����:
//		shaderList : shader�ļ���
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