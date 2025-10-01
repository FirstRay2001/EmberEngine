// Shader.h
// 着色器类
// created by FirstRay2001, Sep/15/2025

#include "Common.h"

class FShader
{
public:
	FShader(const char* VertexPath, const char* FragmentPath);

public:
	// 使用着色器程序
	void Use() const;

	void SetBool(const char* UniformName, bool Value) const;
	void SetInt(const char* UniformName, int Value) const;
	void SetFloat(const char* UniformName, float Value) const;
	void SetTexture(const char* UniformName, const FTexture& Value) const;

public:
	unsigned int ID_;

private:
	void CheckCompileErrors(unsigned int Shader, std::string type);
};