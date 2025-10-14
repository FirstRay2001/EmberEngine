// Mesh.h
// 网格类
// created by FirstRay2001, Sep/15/2025

#pragma once

#include "Common.h"

struct FVertex
{
	MyMath::FVector3 Position;
	MyMath::FVector3 Normal;
	MyMath::FVector2 TexCoords;
};

class FMesh
{
public:
	FMesh(const MySTL::TVector<FVertex>& Vertices, const MySTL::TVector<unsigned int> Indices);
	~FMesh() {};

public:
	void Draw() const;

private:
	GLfloat* ProcessVertex();

private:
	unsigned int					VAO_;
	unsigned int					VBO_;
	unsigned int					EBO_;
	MySTL::TVector<FVertex>			Vertices_;
	MySTL::TVector<unsigned int>	Indices_;

public:
	static FVertex CreateVertex(const MyMath::FVector3& Position, const MyMath::FVector3& Normal, const MyMath::FVector2& TexCoords)
	{
		FVertex Vertex;
		Vertex.Position = Position;
		Vertex.Normal = Normal;
		Vertex.TexCoords = TexCoords;
		return Vertex;
	}

	static FVertex CreateVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		FVertex Vertex;
		Vertex.Position = MyMath::FVector3(x, y, z);
		Vertex.Normal = MyMath::FVector3(nx, ny, nz);
		Vertex.TexCoords = MyMath::FVector2(u, v);
		return Vertex;
	}

	// 从文本文件构造顶点数据
	static void CreateVertex(MySTL::TVector<FVertex>& OutVec, const char* FilePath)
	{
		int Cnt = 0;

		// 读取文件
		MySTL::TUniquePtr<float> data(MyFS::ReadFloatFromFile(FilePath, Cnt));
		if (data.Get() == nullptr || Cnt % 6 != 0)
		{
			OutVec.Clear();
			return;
		}

		for (int i = 0; i < Cnt / 6; i++)
		{
			FVertex v = CreateVertex(
				data.Get()[i * 6 + 0], data.Get()[i * 6 + 1], data.Get()[i * 6 + 2],
				data.Get()[i * 6 + 3], data.Get()[i * 6 + 4], data.Get()[i * 6 + 5],
				0, 0
			);
			OutVec.push_back(v);
		}
	}
};