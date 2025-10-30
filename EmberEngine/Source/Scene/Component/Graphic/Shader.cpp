// Shader.cpp
// 着色器类
// created by FirstRay2001, Sep/15/2025

#include "Shader.h"

FShader::FShader() :
	ID_()
{
}

FShader::FShader(const char* VertexPath, const char* FragmentPath) :
	ID_()
{
	LoadShader(VertexPath, FragmentPath);
}

FShader::FShader(const char* VertexPath, const char* GeometryPath, const char* FragmentPath) :
	ID_()
{
	LoadShader(VertexPath, GeometryPath, FragmentPath);
}

FShader::~FShader()
{
	// 释放着色器程序
	glDeleteProgram(ID_);
}

void FShader::Use() const
{
	glUseProgram(ID_);
}

void FShader::SetBool(const char* UniformName, bool Value) const
{
	Use();

	unsigned int UniformLoc = glGetUniformLocation(ID_, UniformName);
	glUniform1i(UniformLoc, (int)Value);
}

void FShader::SetInt(const char* UniformName, int Value) const
{
	Use();

	unsigned int UniformLoc = glGetUniformLocation(ID_, UniformName);
	glUniform1i(UniformLoc, Value);
}

void FShader::SetFloat(const char* UniformName, float Value) const
{
	Use();

	unsigned int UniformLoc = glGetUniformLocation(ID_, UniformName);
	glUniform1f(UniformLoc, Value);
}

void FShader::SetVector3(const char* UniformName, const MyMath::FVector3& Value) const
{
	Use();

	unsigned int UniformLoc = glGetUniformLocation(ID_, UniformName);
	glUniform3f(UniformLoc, Value[0], Value[1], Value[2]);
}

void FShader::SetTexture(const char* UniformName, const FTexture& Value) const
{
	// glUniform1i(glGetUniformLocation(ID_, UniformName), )
	LOG_WARN("[SHADER] SetTexture not implemented yet");
}

void FShader::SetMatrix(const char* UniformName, const MyMath::FMatrix& Matrix) const
{
	Use();

	unsigned int UniformLoc = glGetUniformLocation(ID_, UniformName);
	glUniformMatrix4fv(UniformLoc, 1, GL_FALSE, Matrix.GetRawData());
}

bool FShader::LoadShader(const char* VertexPath, const char* FragmentPath)
{
	std::string VertexSourceCode, FragmenSourceCode;
	std::ifstream VertexFile, FragmentFile;

	// 确保ifstream object可以抛出异常
	VertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	FragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// 读取Shader源码
	try
	{
		// 打开文件
		VertexFile.open(VertexPath);
		FragmentFile.open(FragmentPath);

		// 读取
		std::stringstream VertexStringStream, FragmentStringStream;
		VertexStringStream << VertexFile.rdbuf();
		FragmentStringStream << FragmentFile.rdbuf();

		// 转换为string
		VertexSourceCode = VertexStringStream.str();
		FragmenSourceCode = FragmentStringStream.str();

		// 关闭句柄
		VertexFile.close();
		FragmentFile.close();
	}
	catch (std::ifstream::failure& e)
	{
		LOG_ERROR("[SHADER] file not successfully read: %s", e.what());
		return false;
	}

	unsigned int VertexShader, FragmentShader;
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// 编译shader源码
	const char* c_vertex = VertexSourceCode.c_str();
	const char* c_fragment = FragmenSourceCode.c_str();
	glShaderSource(VertexShader, 1, &c_vertex, NULL);
	glShaderSource(FragmentShader, 1, &c_fragment, NULL);
	glCompileShader(VertexShader);
	glCompileShader(FragmentShader);

	// 检查编译错误
	CheckCompileErrors(VertexShader, "VERTEX");
	CheckCompileErrors(FragmentShader, "FRAGMENT");

	// 链接shader
	ID_ = glCreateProgram();
	glAttachShader(ID_, VertexShader);
	glAttachShader(ID_, FragmentShader);
	glLinkProgram(ID_);

	// 检查链接错误
	CheckCompileErrors(ID_, "PROGRAM");

	// 释放资源
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	return true;
}

bool FShader::LoadShader(const char* VertexPath, const char* GeometryPath, const char* FragmentPath)
{
	std::string VertexSourceCode, GeometrySourceCode, FragmenSourceCode;
	std::ifstream VertexFile, GeometryFile, FragmentFile;

	// 确保ifstream object可以抛出异常
	VertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	GeometryFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	FragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// 读取Shader源码
	try
	{
		// 打开文件
		VertexFile.open(VertexPath);
		GeometryFile.open(GeometryPath);
		FragmentFile.open(FragmentPath);

		// 读取
		std::stringstream VertexStringStream, GeometryStringStream, FragmentStringStream;
		VertexStringStream << VertexFile.rdbuf();
		GeometryStringStream << GeometryFile.rdbuf();
		FragmentStringStream << FragmentFile.rdbuf();

		// 转换为string
		VertexSourceCode = VertexStringStream.str();
		GeometrySourceCode = GeometryStringStream.str();
		FragmenSourceCode = FragmentStringStream.str();

		// 关闭句柄
		VertexFile.close();
		GeometryFile.close();
		FragmentFile.close();
	}
	catch (std::ifstream::failure& e)
	{
		LOG_ERROR("[SHADER] file not successfully read: %s", e.what());
		return false;
	}

	unsigned int VertexShader, GeometryShader, FragmentShader;
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// 编译shader源码
	const char* c_vertex = VertexSourceCode.c_str();
	const char* c_geometry = GeometrySourceCode.c_str();
	const char* c_fragment = FragmenSourceCode.c_str();

	glShaderSource(VertexShader, 1, &c_vertex, NULL);
	glShaderSource(GeometryShader, 1, &c_geometry, NULL);
	glShaderSource(FragmentShader, 1, &c_fragment, NULL);
	glCompileShader(VertexShader);
	glCompileShader(GeometryShader);
	glCompileShader(FragmentShader);

	// 检查编译错误
	CheckCompileErrors(VertexShader, "VERTEX");
	CheckCompileErrors(GeometryShader, "GEOMETRY");
	CheckCompileErrors(FragmentShader, "FRAGMENT");

	// 链接shader
	ID_ = glCreateProgram();
	glAttachShader(ID_, VertexShader);
	glAttachShader(ID_, GeometryShader);
	glAttachShader(ID_, FragmentShader);
	glLinkProgram(ID_);

	// 检查链接错误
	CheckCompileErrors(ID_, "PROGRAM");

	// 释放资源
	glDeleteShader(VertexShader);
	glDeleteShader(GeometryShader);
	glDeleteShader(FragmentShader);

	return true;
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
