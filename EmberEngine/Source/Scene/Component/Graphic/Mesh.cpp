// Mesh.cpp
// ������
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

	// ��VAO
	glBindVertexArray(VAO_);

	// ���붥������
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	Vertices_.Shrink();
	glBufferData(GL_ARRAY_BUFFER, Vertices_.Size() * sizeof(FVertex), Vertices_.GetRawData(), GL_STATIC_DRAW);

	// ����λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// ���㷨������
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// ����UV����
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// ���붥������
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
