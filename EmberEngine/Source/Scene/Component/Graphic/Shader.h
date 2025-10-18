// Shader.h
// 着色器类
// created by FirstRay2001, Sep/15/2025

#pragma once

#include "Common.h"

class FShader
{
public:
	FShader();
	FShader(const char* VertexPath, const char* FragmentPath);

public:
	// 使用着色器程序
	void Use() const;

	void SetBool(const char* UniformName, bool Value) const;
	void SetInt(const char* UniformName, int Value) const;
	void SetFloat(const char* UniformName, float Value) const;
	void SetVector3(const char* UniformName, const MyMath::FVector3& Value) const;
	void SetTexture(const char* UniformName, const FTexture& Value) const;

	// 设置Uniform矩阵，传入行优先矩阵
	void SetMatrix(const char* UniformName, const MyMath::FMatrix& Matrix) const;

private:
	bool LoadShader(const char* VertexPath, const char* FragmentPath);

public:
	unsigned int ID_;

private:
	void CheckCompileErrors(unsigned int Shader, std::string type);
};