// Mesh.cpp
// 网格类
// created by FirstRay2001, Sep/15/2025

#include "Mesh.h"
#include "Shader.h"

FMesh::FMesh() :
	VAO_(),
	VBO_(),
	EBO_(),
	Vertices_(),
	Indices_()
{
}

FMesh::FMesh(const MySTL::TVector<FVertex>& Vertices, const MySTL::TVector<unsigned int> Indices) :
	VAO_(),
	VBO_(),
	EBO_(),
	Vertices_(Vertices),
	Indices_(Indices)
{
	glGenVertexArrays(1, &VAO_);
	glGenBuffers(1, &VBO_);
	glGenBuffers(1, &EBO_);

	// 绑定VAO
	glBindVertexArray(VAO_);

	// 传入顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	Vertices_.Shrink();
	glBufferData(GL_ARRAY_BUFFER, Vertices_.Size() * sizeof(FVertex), Vertices_.GetRawData(), GL_STATIC_DRAW);

	// 顶点位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FVertex), (void*)0);
	glEnableVertexAttribArray(0);

	// 顶点法线属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FVertex), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// 顶点UV坐标
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(FVertex), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// 切线
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(FVertex), (void*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	// 副切线
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(FVertex), (void*)(11 * sizeof(GLfloat)));
	glEnableVertexAttribArray(4);

	// 骨骼ID
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(FVertex), (void*)offsetof(FVertex, BoneID));
	glEnableVertexAttribArray(5);

	// 骨骼权重
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(FVertex), (void*)offsetof(FVertex,BoneWeight));
	glEnableVertexAttribArray(6);

	// 传入顶点索引
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
	Indices_.Shrink();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices_.Size() * sizeof(unsigned int), Indices_.GetRawData(), GL_STATIC_DRAW);
}

FMesh::~FMesh()
{
	// 释放OpenGL资源
	glDeleteBuffers(1, &VBO_);
	glDeleteBuffers(1, &EBO_);
	glDeleteVertexArrays(1, &VAO_);
}

void FMesh::Draw() const
{
	glBindVertexArray(VAO_);
	glDrawElements(GL_TRIANGLES, Indices_.Size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

GLfloat* FMesh::ProcessVertex()
{
	/*float* Ret = new float[Vertices_.Size() * 8];
	for (size_t i = 0; i < Vertices_.Size(); i++)
	{
		FVertex CurVertex = Vertices_[i];
		size_t CurPos = i * 8 * sizeof(float);
		memcpy(Ret + CurPos, CurVertex.Position.GetRawData(), 3 * sizeof(float));
		memcpy(Ret + CurPos + 3 * sizeof(float), CurVertex.Normal.GetRawData(), 3 * sizeof(float));
		memcpy(Ret + CurPos + 6 * sizeof(float), CurVertex.TexCoords.GetRawData(), 2 * sizeof(float));
	}
	return Ret;*/
	return nullptr;
}

FMesh FMesh::CreateCube(float Size, MyMath::FVector3 Offset, int BoneId)
{
	// 立方体顶点数据
	MySTL::TVector<FVertex> Vertices;

	float HalfSize = Size / 2.0f;

	Vertices.push_back(CreateVertex(-HalfSize, -HalfSize, -HalfSize, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f));
	Vertices.push_back(CreateVertex(HalfSize, -HalfSize, -HalfSize, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f));
	Vertices.push_back(CreateVertex(HalfSize, HalfSize, -HalfSize, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f));
	Vertices.push_back(CreateVertex(-HalfSize, HalfSize, -HalfSize, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f));
	Vertices.push_back(CreateVertex(-HalfSize, -HalfSize, HalfSize, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f));
	Vertices.push_back(CreateVertex(HalfSize, -HalfSize, HalfSize, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f));
	Vertices.push_back(CreateVertex(HalfSize, HalfSize, HalfSize, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));
	Vertices.push_back(CreateVertex(-HalfSize, HalfSize, HalfSize, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f));

	for (int i = 0; i < Vertices.Size(); i++)
	{
		Vertices[i].Position = Vertices[i].Position + Offset;
		Vertices[i].BoneID[0] = BoneId;
		Vertices[i].BoneWeight[0] = 1;
	}

	// 立方体索引数据
	MySTL::TVector<unsigned int> Indices;
	Indices.push_back(0); Indices.push_back(2); Indices.push_back(1);
	Indices.push_back(2); Indices.push_back(0); Indices.push_back(3);
	Indices.push_back(4); Indices.push_back(5); Indices.push_back(6);
	Indices.push_back(6); Indices.push_back(7); Indices.push_back(4);
	Indices.push_back(0); Indices.push_back(4); Indices.push_back(7);
	Indices.push_back(7); Indices.push_back(3); Indices.push_back(0);
	Indices.push_back(1); Indices.push_back(6); Indices.push_back(5);
	Indices.push_back(6); Indices.push_back(1); Indices.push_back(2);
	Indices.push_back(3); Indices.push_back(6); Indices.push_back(2);
	Indices.push_back(6); Indices.push_back(3); Indices.push_back(7);
	Indices.push_back(0); Indices.push_back(1); Indices.push_back(5);
	Indices.push_back(5); Indices.push_back(4); Indices.push_back(0);

	return FMesh(Vertices, Indices);
}
