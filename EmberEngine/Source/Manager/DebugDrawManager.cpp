// DebugDrawManager.cpp
// Debug绘制管理器
// created by FirstRay2001, Oct/28/2025

#include "DebugDrawManager.h"
#include "Source/Scene/Component/Graphic/Mesh.h"

void MDebugDrawManager::Initialize()
{
	// Debug Line
	{
		glGenVertexArrays(1, &LinesVAO_);
		glGenBuffers(1, &LinesVBO_);

		glBindVertexArray(LinesVAO_);
		glBindBuffer(GL_ARRAY_BUFFER, LinesVBO_);

		// 位置属性
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// 颜色属性
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}
}

void MDebugDrawManager::Tick()
{
	DebugLines_.Clear();
}

void MDebugDrawManager::DrawInWorldSpace()
{
}

void MDebugDrawManager::AddLine(const MyMath::FVector3& Start, const MyMath::FVector3& End, const MyMath::FVector3& Color)
{
	FDebugPoint NewPointStart, NewPointEnd;
	NewPointStart.Position = Start;
	NewPointEnd.Position = End;
	NewPointStart.Color = Color;
	NewPointEnd.Color = Color;
	DebugLines_.push_back(NewPointStart);
	DebugLines_.push_back(NewPointEnd);
}

void MDebugDrawManager::DrawLines()
{
	glBindBuffer(GL_ARRAY_BUFFER, LinesVBO_);
	glBufferData(GL_ARRAY_BUFFER, DebugLines_.Size() * sizeof(FDebugPoint), DebugLines_.GetRawData(), GL_DYNAMIC_DRAW);
	glBindVertexArray(LinesVAO_);
	glDrawArrays(GL_LINES, 0, DebugLines_.Size());
	glBindVertexArray(0);
}
