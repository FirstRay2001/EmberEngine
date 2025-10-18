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
	MySTL::TVector<MySTL::TSharedPtr<AActor>>	Actors_;
	MySTL::TSharedPtr<ACameraActor>				CameraActor_;
	MySTL::TSharedPtr<AModelActor>				TestModelActor_;
	MySTL::TSharedPtr<APointLightActor>			PointLightActor;
	MySTL::TSharedPtr<ADirectionalLightActor>		DirectionalLightActor_;
};