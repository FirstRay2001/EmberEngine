// ForwardRenderer.h
// 前向渲染管线
// Created by FirstRay2001, Oct/19/2025

#pragma once

#include "Common.h"

class FForwardRenderer
{
public:
	FForwardRenderer(unsigned int ScreenWidth = 1600, unsigned int ScreenHeight = 1200);
	~FForwardRenderer();

public:
	// 初始化
	void Initialize();

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
	void InitialzeShadowMap();

	void InitializeOutline();

	void InitializeSkybox();

	void InitializePostProcess();

	void InitializeDebug();

	// 更新平行光阴影贴图
	void UpdateDirectionalLightShadowMaps();

	// 更新点光源阴影贴图
	void UpdatePointLightShadowMaps();

	// 绘制天空盒
	void RenderSkybox();

	// 绘制模型
	void RenderModels();

	// 绘制轮廓、法线
	void RenderOutline();

	// Debug绘制
	void RenderDebug();

	// 后处理
	void PostProcess();

	// 绘制UI
	void RenderUI();

	// 启用模板写入
	void EnableStencilWrite();

	// 禁用模板写入
	void DisableStencilWrite();

private:
	MySTL::TWeakPtr<ACameraActor>						CurrentCamera_;
	MySTL::TWeakPtr<ADirectionalLightActor>				DirectionalLight_;
	MySTL::TVector<MySTL::TWeakPtr<APointLightActor>>	PointLights_;
	MySTL::TVector<MySTL::TWeakPtr<AModelActor>>		Models_;
	MySTL::TUniquePtr<FMesh>							SkyboxMesh_;
	MySTL::TUniquePtr<FMesh>							ScreenQuadMesh_;
	MyMath::FMatrix										DirectionalLightSpaceMatrix_;
	unsigned int SCREEN_WIDTH;
	unsigned int SCREEN_HEIGHT;
	unsigned int SHADOW_WIDTH;
	unsigned int SHADOW_HEIGHT;
	unsigned int DirectionalShadowMapFBO_;
	unsigned int PointShadowMapFBO_;
	unsigned int SceneColorFBO_;
	float PointLightShadowFarPlane_ = 25.0f;
};
