// ForwardRenderer.cpp
// ǰ����Ⱦ����
// Created by FirstRay2001, Oct/19/2025

#include "ForwardRenderer.h"
#include "Source/Scene/Actor/ModelActor.h"
#include "Source/Scene/Actor/CameraActor.h"
#include "Source/Scene/Actor/PointLightActor.h"
#include "Source/Scene/Actor/DirectionalLightActor.h"
#include "Source/Manager/ShaderManager.h"
#include "Source/Manager/TextureManager.h"
#include "Source/Scene/Component/Graphic/Shader.h"

FForwardRenderer::FForwardRenderer() :
	SCREEN_WIDTH(1600),
	SCREEN_HEIGHT(1200),
	SHADOW_WIDTH(1024),
	SHADOW_HEIGHT(1024),
	ShadowMapTex_(),
	DepthMapFBO_()
{
}

FForwardRenderer::~FForwardRenderer()
{
}

void FForwardRenderer::Initialize(unsigned int ScreenWidth, unsigned int ScreenHeight)
{
	MShaderManager::GetInstance().LoadShader("ShadowMap", "ShadowMap/SimpleShadowMap.vert", "ShadowMap/SimpleShadowMap.frag");
	SCREEN_WIDTH = ScreenWidth;
	SCREEN_HEIGHT = ScreenHeight;

	// ����FrameBuffer
	glGenFramebuffers(1, &DepthMapFBO_);

	// ��Ӱ��ͼ��С
	SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	// ���������ͼ
	glGenTextures(1, &ShadowMapTex_);
	glBindTexture(GL_TEXTURE_2D, ShadowMapTex_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// ��FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMapTex_, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FForwardRenderer::RenderScene()
{
	// Shadow Pass
	UpdateShadowMaps();

	// Base Pass
	for (int i = 0; i < Models_.Size(); i++)
	{
		auto ModelActor = Models_[i];
		ModelActor->SetCamera(*CurrentCamera_);

		// ƽ�й�
		if (DirectionalLight_.IsValid())
		{
			ModelActor->SetDirectionalLight(*DirectionalLight_.Get());
		}

		// TODO: Ŀǰֻ֧��һ�����Դ������֧�ֶ�����Դ
		for (int j = 0; j < PointLights_.Size(); j++)
		{
			auto PointLightActor = PointLights_[j];
			//ModelActor->SetPointLight(*PointLightActor);
			CurrentCamera_->RenderPointLight(PointLightActor.Get());
		}

		MTextureManager::GetInstance().UseShadowMap(*ModelActor->GetShader(), ShadowMapTex_);
		ModelActor->SetLightSpaceMatrix(LightSpaceMatrix_);
		ModelActor->Draw();
	}
}

void FForwardRenderer::Shutdown()
{
	// ��ճ�������
	ClearSceneData();
}

void FForwardRenderer::SetCurrentCamera(const MySTL::TSharedPtr<ACameraActor>& InCamera)
{
	CurrentCamera_ = InCamera;
}

void FForwardRenderer::SetDirectionalLight(const MySTL::TSharedPtr<ADirectionalLightActor>& InDirectionalLight)
{
	DirectionalLight_ = InDirectionalLight;
}

void FForwardRenderer::AddPointLight(const MySTL::TSharedPtr<APointLightActor>& InPointLight)
{
	PointLights_.push_back(InPointLight);
}

void FForwardRenderer::AddModel(const MySTL::TSharedPtr<AModelActor>& InModel)
{
	Models_.push_back(InModel);
}

void FForwardRenderer::ClearSceneData()
{
	CurrentCamera_.Reset();
	PointLights_.Clear();
	Models_.Clear();
}

void FForwardRenderer::UpdateShadowMaps()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO_);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ShadowMapTex_);

	// �����Դ�ռ����
	MyMath::FMatrix OrthographicProjection = MyMath::GetOrthoProjection(-5.0f, 5.0f, -5.0f, 5.0f, 
		CurrentCamera_->GetNearClip(), CurrentCamera_->GetFarClip());
	MyMath::FMatrix LightView = MyMath::GetLookAt(
		DirectionalLight_->GetDirection() * -3.0f,
		MyMath::FVector3(0.0f, 0.0f, 0.0f),
		MyMath::FVector3(0.0f, 1.0f, 0.0f)
	);
	LightSpaceMatrix_ = OrthographicProjection * LightView;

	// ʹ����Ӱ��ͼShader
	auto ShadowMapShader = MShaderManager::GetInstance().GetShader("ShadowMap");
	ShadowMapShader->Use();
	ShadowMapShader->SetMatrix("lightSpaceMatrix", LightSpaceMatrix_.TransposeConst());

	// ���Ƴ����е�����ģ����������Ӱ��ͼ
	for (int i = 0; i < Models_.Size(); i++)
	{
		auto ModelActor = Models_[i];
		ShadowMapShader->SetMatrix("model", ModelActor->GetWorldMatrix().Transpose());
		ModelActor->RawDraw();
	}

	// ���FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// �ָ��ӿڴ�С
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// �����Ȼ�����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
