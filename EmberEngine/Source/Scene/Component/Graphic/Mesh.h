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
	MyMath::FVector3 Tangent;
	MyMath::FVector3 BiTangent;
};

class FMesh
{
public:
	FMesh();
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
	static FVertex CreateVertex(const MyMath::FVector3& Position, const MyMath::FVector3& Normal, 
		const MyMath::FVector2& TexCoords, const MyMath::FVector3& Tangnet = MyMath::FVector3(1,0,0), const MyMath::FVector3& BiTangent = MyMath::FVector3(0,1,0))
	{
		FVertex Vertex;
		Vertex.Position = Position;
		Vertex.Normal = Normal;
		Vertex.TexCoords = TexCoords;
		Vertex.Tangent = Tangnet;
		Vertex.BiTangent = BiTangent;
		return Vertex;
	}

	static FVertex CreateVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		FVertex Vertex;
		Vertex.Position = MyMath::FVector3(x, y, z);
		Vertex.Normal = MyMath::FVector3(nx, ny, nz);
		Vertex.TexCoords = MyMath::FVector2(u, v);
		Vertex.Tangent = MyMath::FVector3(1, 0, 0);
		Vertex.BiTangent = MyMath::FVector3(0, 1, 0);
		return Vertex;
	}

	// 从文本文件构造顶点数据
	static void CreateVertex(MySTL::TVector<FVertex>& OutVec, const char* FilePath)
	{
		int Cnt = 0;

		// 读取文件
		MySTL::TUniquePtr<float> data(MyFS::ReadFloatFromFile(FilePath, Cnt));
		if (data.Get() == nullptr || Cnt % 8 != 0)
		{
			OutVec.Clear();
			return;
		}

		for (int i = 0; i < Cnt / 8; i++)
		{
			FVertex v = CreateVertex(
				data.Get()[i * 8 + 0], data.Get()[i * 8 + 1], data.Get()[i * 8 + 2],
				data.Get()[i * 8 + 3], data.Get()[i * 8 + 4], data.Get()[i * 8 + 5],
				data.Get()[i * 8 + 6], data.Get()[i * 8 + 7]
			);
			OutVec.push_back(v);
		}
	}

	// 创建立方体
	static FMesh CreateCube(float Size);
};