// Mesh.h
// Õ¯∏Ò¿‡
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
};