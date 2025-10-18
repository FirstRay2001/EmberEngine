// Scene.h
// 场景类
// created by FirstRay2001, Sep/15/2025

#pragma once

#include "Common.h"

class FScene
{
public:
	FScene();
	~FScene() {};

public:
	// 载入场景
	void Load();

	// 卸载场景
	void Unload();

	// 驱动游戏逻辑
	void Tick(float DeltaTime);

	// 渲染场景
	void Render();

private:
	MySTL::TVector<MySTL::TSharedPtr<AActor>>	Actors_;
	MySTL::TSharedPtr<ACameraActor>				CameraActor_;
	MySTL::TSharedPtr<AModelActor>				TestModelActor_;
	MySTL::TSharedPtr<APointLightActor>			PointLightActor;
	MySTL::TSharedPtr<ADirectionalLightActor>		DirectionalLightActor_;
};