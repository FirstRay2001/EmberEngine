// ForwardRenderer.h
// 前向渲染管线
// Created by FirstRay2001, Oct/19/2025

#pragma once

#include "Common.h"

class FForwardRenderer
{
public:
	FForwardRenderer();
	~FForwardRenderer();

public:
	// 初始化
	void Initialize(unsigned int ScreenWidth = 1600, unsigned int ScreenHeight = 1200);

	// 渲染场景
	void RenderScene();

	// 关闭渲染器
	void Shutdown();

	// 设置当前摄像机
	void SetCurrentCamera(const MySTL::TSharedPtr<ACameraActor>& InCamera);

	// 设置平行光源
	void SetDirectionalLight(const MySTL::TSharedPtr<ADirectionalLightActor>& InDirectionalLight);

	// 添加点光源
	void AddPointLight(const MySTL::TSharedPtr<APointLightActor>& InPointLight);

	// 添加模型
	void AddModel(const MySTL::TSharedPtr<AModelActor>& InModel);

	// 清除场景数据
	void ClearSceneData();

private:
	// 获取阴影贴图
	void UpdateShadowMaps();

private:
	MySTL::TWeakPtr<ACameraActor>						CurrentCamera_;
	MySTL::TWeakPtr<ADirectionalLightActor>				DirectionalLight_;
	MySTL::TVector<MySTL::TWeakPtr<APointLightActor>>	PointLights_;
	MySTL::TVector<MySTL::TWeakPtr<AModelActor>>		Models_;
	unsigned int SCREEN_WIDTH;
	unsigned int SCREEN_HEIGHT;
	unsigned int SHADOW_WIDTH;
	unsigned int SHADOW_HEIGHT;
	unsigned int ShadowMapTex_;
	unsigned int DepthMapFBO_;
	MyMath::FMatrix LightSpaceMatrix_;
};
