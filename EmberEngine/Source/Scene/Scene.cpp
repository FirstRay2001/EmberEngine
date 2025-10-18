// Scene.cpp
// 场景类
// created by FirstRay2001, Sep/15/2025

#include "Scene.h"
#include "Source/Scene/Component/Graphic/Mesh.h"
#include "Source/Scene/Component/Graphic/Shader.h"
#include "Source/Scene/Component/Graphic/Material.h"
#include "Source/Scene/Actor/MeshActor.h"
#include "Source/Scene/Actor/ModelActor.h"
#include "Source/Scene/Actor/CameraActor.h"
#include "Source/Scene/Actor/PointLightActor.h"
#include "Source/Scene/Actor/DirectionalLightActor.h"
#include "Source/Scene/Component/Graphic/Texture.h"
#include "Source/Manager/TextureManager.h"
#include "Source/Manager/ModelManager.h"
#include "Source/Manager/ShaderManager.h"

#include "Source/Scene/Component/Graphic/Model.h"

FScene::FScene()
{
}

void FScene::Load()
{
	//////// TEST ////////
	// 点光源模型
	MySTL::TVector<FVertex> Vertices;
	FMesh::CreateVertex(Vertices, "D:/Dev/Project/EmberEngine/EmberEngine/Resources/Text/VertexData.txt");
	MySTL::TVector<unsigned int> Indices;
	int IndicesCnt = 0;
	MySTL::TUniquePtr<int> TempData(MyFS::ReadIntFromFile("D:/Dev/Project/EmberEngine/EmberEngine/Resources/Text/IndexData.txt", IndicesCnt));
	for (int i = 0; i < IndicesCnt; i++)
		Indices.push_back(TempData.Get()[i]);
	FMesh* Mesh = new FMesh(Vertices, Indices);

	// 点光源Shader
	int LightShaderIndex = MShaderManager::GetInstance().LoadShader("LightBall", "LightBall/LightBallVert.vert", "LightBall/LightBallFrag.frag");
	auto LightShaderPtr = MShaderManager::GetInstance().GetShader(LightShaderIndex);

	// 光源Actor
	PointLightActor = MySTL::TSharedPtr<APointLightActor>(new APointLightActor(MySTL::TSharedPtr<FMesh>(Mesh), LightShaderPtr));
	//Actors_.push_back(PointLightActor);
	PointLightActor->SetActorWorldLocation(MyMath::FVector3(0, 2, 1));
	PointLightActor->SetActorWorldScale(MyMath::FVector3(0.2, 0.2, 0.2));
	PointLightActor->SetAmbientColor(MyMath::FVector3(0.05f, 0.05f, 0.05f));
	PointLightActor->SetDiffuseColor(MyMath::FVector3(0.5f, 0.5f, 0.5f));
	PointLightActor->SetSpecularColor(MyMath::FVector3(1.0f, 1.0f, 1.0f));
	DirectionalLightActor_ = MySTL::TSharedPtr<ADirectionalLightActor>(new ADirectionalLightActor());
	DirectionalLightActor_->SetDirection(MyMath::FVector3(-0.2f, -1.0f, -0.3f));
	DirectionalLightActor_->SetAmbientColor(MyMath::FVector3(0.1f, 0.1f, 0.15f));

	// 相机Actor
	CameraActor_ = MySTL::TSharedPtr<ACameraActor>(new ACameraActor(45.0f, 8.0f / 6.0f, 0.1f, 100.0f));
	//Actors_.push_back(CameraActor_);
	CameraActor_->SetActorWorldLocation(MyMath::FVector3(0, 0, 5));
	CameraActor_->SetActorWorldRotation(FRotator(float(0 * MyMath::Deg2Rad), MyMath::FVector3(0, 1, 0)));

	// 背包模型
	int ModelIndex = MModelManager::GetInstance().LoadModel("backpack/backpack.obj");
	auto ModelPtr = MModelManager::GetInstance().GetModel(ModelIndex);
	int ShaderIndex = MShaderManager::GetInstance().LoadShader("BlinnPhong", "BlinnPhong/PhongVert.vert", "BlinnPhong/PhongFrag.frag");
	auto TestShader_ = MShaderManager::GetInstance().GetShader(ShaderIndex);
	TestModelActor_ = MySTL::TSharedPtr<AModelActor>(new AModelActor(ModelPtr, TestShader_));
	//Actors_.push_back(TestModelActor_);
}

void FScene::Unload()
{
}

void FScene::Tick(float DeltaTime)
{
	CameraActor_->Tick(DeltaTime);
	TestModelActor_->Tick(DeltaTime);
}

void FScene::Render() 
{
	TestModelActor_->SetCamera(*CameraActor_);
	TestModelActor_->SetPointLight(*PointLightActor);
	TestModelActor_->SetDirectionalLight(*DirectionalLightActor_);

	TestModelActor_->Draw();
	
	CameraActor_->RenderPointLight(PointLightActor.Get());
}
