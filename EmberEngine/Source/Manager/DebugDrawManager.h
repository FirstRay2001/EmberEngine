// DebugDrawManager.h
// Debug绘制管理器
// created by FirstRay2001, Oct/28/2025

#pragma once

#include "Common.h"

class MDebugDrawManager
{
public:
	// 单例
	static MDebugDrawManager& GetInstance()
	{
		static MDebugDrawManager Instance;
		return Instance;
	}

public:
	void Initialize();

	void Tick();

	void DrawInWorldSpace();

	void AddLine(const MyMath::FVector3& Start, const MyMath::FVector3& End, const MyMath::FVector3& Color = MyMath::FVector3(1, 0, 0));

	void DrawLines();

public:
	struct FDebugPoint
	{
		MyMath::FVector3 Position;
		MyMath::FVector3 Color;
	};

private:
	MySTL::TUniquePtr<FMesh> SphereMesh_;
	MySTL::TVector<FDebugPoint> DebugLines_;
	GLuint	LinesVBO_;
	GLuint	LinesVAO_;
};
