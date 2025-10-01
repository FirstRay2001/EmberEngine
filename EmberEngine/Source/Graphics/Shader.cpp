// Shader.cpp
// ��ɫ����
// created by FirstRay2001, Sep/15/2025

#include "Shader.h"

FShader::FShader(const char* VertexPath, const char* FragmentPath) :
	ID_()
{
	std::string VertexSourceCode, FragmenSourceCode;
	std::ifstream VertexFile, FragmentFile;

	// ȷ��ifstream object�����׳��쳣
	VertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	FragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// ��ȡShaderԴ��
	try
	{
		// ���ļ�
		VertexFile.open(VertexPath);
		FragmentFile.open(FragmentPath);

		// ��ȡ
		std::stringstream VertexStringStream, FragmentStringStream;
		VertexStringStream << VertexFile.rdbuf();
		FragmentStringStream << FragmentFile.rdbuf();

		// ת��Ϊstring
		VertexSourceCode = VertexStringStream.str();
		FragmenSourceCode = FragmentStringStream.str();

		// �رվ��
		VertexFile.close();
		FragmentFile.close();
	}
	catch (std::ifstream::failure& e)
	{
		LOG_ERROR("[SHADER] file not successfully read: %s", e.what());
	}

	unsigned int VertexShader, FragmentShader;
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// ����shaderԴ��
	const char* c_vertex = VertexSourceCode.c_str();
	const char* c_fragment = FragmenSourceCode.c_str();
	glShaderSource(VertexShader, 1, &c_vertex, NULL);
	glShaderSource(FragmentShader, 1, &c_fragment, NULL);
	glCompileShader(VertexShader);
	glCompileShader(FragmentShader);

	// ���������
	CheckCompileErrors(VertexShader, "VERTEX");
	CheckCompileErrors(FragmentShader, "FRAGMENT");
	
	// ����shader
	ID_ = glCreateProgram();
	glAttachShader(ID_, VertexShader);
	glAttachShader(ID_, FragmentShader);
	glLinkProgram(ID_);

	// ������Ӵ���
	CheckCompileErrors(ID_, "PROGRAM");

	// �ͷ���Դ
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
}

void FShader::Use() const
{
	glUseProgram(ID_);
}

void FShader::SetBool(const char* UniformName, bool Value) const
{
}

void FShader::SetInt(const char* UniformName, int Value) const
{
}

void FShader::SetFloat(const char* UniformName, float Value) const
{
}

void FShader::SetTexture(const char* UniformName, const FTexture& Value) const
{
	// glUniform1i(glGetUniformLocation(ID_, UniformName), )
}

void FShader::CheckCompileErrors(unsigned int Shader, std::string type)
{
	int Success;
	GLchar InfoLog[1024];

	if (type == "PROGRAM")
	{
		glGetProgramiv((GLuint)Shader, GL_LINK_STATUS, &Success);
		if (!Success)
		{
			glGetProgramInfoLog((GLuint)Shader, 1024, NULL, InfoLog);
			LOG_ERROR("[SHADER] program linking error: %s", InfoLog);
		}
	}
	else
	{
		glGetShaderiv((GLuint)Shader, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			glGetShaderInfoLog((GLuint)Shader, 1024, NULL, InfoLog);
			LOG_ERROR("[SHADER] shader compilation error of type %s:\n%s", type.c_str(), InfoLog);
		}
	}
}
