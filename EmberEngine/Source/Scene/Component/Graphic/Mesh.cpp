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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// 顶点法线属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// 顶点UV坐标
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// 传入顶点索引
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
	Indices_.Shrink();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices_.Size() * sizeof(unsigned int), Indices_.GetRawData(), GL_STATIC_DRAW);
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
