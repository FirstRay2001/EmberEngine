// ForwardRenderer.h
// ǰ����Ⱦ����
// Created by FirstRay2001, Oct/19/2025

#pragma once

#include "Common.h"

class FForwardRenderer
{
public:
	FForwardRenderer();
	~FForwardRenderer();

public:
	// ��ʼ��
	void Initialize(unsigned int ScreenWidth = 1600, unsigned int ScreenHeight = 1200);

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
	// ��ȡ��Ӱ��ͼ
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
