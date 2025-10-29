// ForwardRenderer.h
// ǰ����Ⱦ����
// Created by FirstRay2001, Oct/19/2025

#pragma once

#include "Common.h"

class FForwardRenderer
{
public:
	FForwardRenderer(unsigned int ScreenWidth = 1600, unsigned int ScreenHeight = 1200);
	~FForwardRenderer();

public:
	// ��ʼ��
	void Initialize();

	// ��Ⱦ����
	void RenderScene();

	// �ر���Ⱦ��
	void Shutdown();

	// ���õ�ǰ�����
	void SetCurrentCamera(const MySTL::TSharedPtr<ACameraActor>& InCamera);

	// ����ƽ�й�Դ
	void SetDirectionalLight(const MySTL::TSharedPtr<ADirectionalLightActor>& InDirectionalLight);

	// ��ӵ��Դ
	void AddPointLight(const MySTL::TSharedPtr<APointLightActor>& InPointLight);

	// ���ģ��
	void AddModel(const MySTL::TSharedPtr<AModelActor>& InModel);

	// �����������
	void ClearSceneData();

private:
	void InitialzeShadowMap();

	void InitializeOutline();

	void InitializeSkybox();

	void InitializePostProcess();

	void InitializeDebug();

	// ����ƽ�й���Ӱ��ͼ
	void UpdateDirectionalLightShadowMaps();

	// ���µ��Դ��Ӱ��ͼ
	void UpdatePointLightShadowMaps();

	// ������պ�
	void RenderSkybox();

	// ����ģ��
	void RenderModels();

	// ��������������
	void RenderOutline();

	// Debug����
	void RenderDebug();

	// ����
	void PostProcess();

	// ����UI
	void RenderUI();

	// ����ģ��д��
	void EnableStencilWrite();

	// ����ģ��д��
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
