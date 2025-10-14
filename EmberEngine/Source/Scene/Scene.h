// Scene.h
// ������
// created by FirstRay2001, Sep/15/2025

#pragma once

#include "Common.h"

class FScene
{
public:
	FScene();
	~FScene() {};

public:
	// ���볡��
	void Load();

	// ж�س���
	void Unload();

	// ������Ϸ�߼�
	void Tick(float DeltaTime);

	// ��Ⱦ����
	void Render();

private:
	MySTL::TUniquePtr<ACameraActor> CameraActor_;
	MySTL::TUniquePtr<AMeshActor>	MeshActor_;
	MySTL::TUniquePtr<AMeshActor>	LightActor_;
};