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
#include "Source/Scene/Renderer/ForwardRenderer.h"

FScene::FScene()
{
}

void FScene::Load()
{
	// 初始化渲染管线
	ForwardRenderer_ = MySTL::TUniquePtr<FForwardRenderer>(new FForwardRenderer(1600, 1200));

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
	PointLightActor_ = MySTL::TSharedPtr<APointLightActor>(new APointLightActor(MySTL::TSharedPtr<FMesh>(Mesh), LightShaderPtr));
	PointLightActor_->SetActorWorldLocation(MyMath::FVector3(1, 1, 1));
	PointLightActor_->SetActorWorldScale(MyMath::FVector3(0.2, 0.2, 0.2));
	PointLightActor_->SetAmbientColor(MyMath::FVector3(0.05f, 0.05f, 0.05f));
	PointLightActor_->SetDiffuseColor(MyMath::FVector3(0.45, 0.45, 0.45));
	PointLightActor_->SetSpecularColor(MyMath::FVector3(1.0f, 1.0f, 1.0f));

	// 将点光源加入渲染管线
	ForwardRenderer_->AddPointLight(PointLightActor_);

	DirectionalLightActor_ = MySTL::TSharedPtr<ADirectionalLightActor>(new ADirectionalLightActor());
	DirectionalLightActor_->SetDirection(MyMath::FVector3(-0.2f, -0.1f, -0.8f));
	DirectionalLightActor_->SetActorWorldLocation(DirectionalLightActor_->GetDirection() * -5.0f);
	DirectionalLightActor_->SetAmbientColor(MyMath::FVector3(0.1f, 0.1f, 0.15f));
	DirectionalLightActor_->SetDiffuseColor(MyMath::FVector3(0.6, 0.6, 0.3));

	// 将平行光加入渲染管线
	ForwardRenderer_->SetDirectionalLight(DirectionalLightActor_);

	// 相机Actor
	CameraActor_ = MySTL::TSharedPtr<ACameraActor>(new ACameraActor(45.0f, 8.0f / 6.0f, 0.1f, 100.0f));
	CameraActor_->SetActorWorldLocation(MyMath::FVector3(0, 0, 5));
	CameraActor_->SetActorWorldRotation(FRotator(float(0 * MyMath::Deg2Rad), MyMath::FVector3(0, 1, 0)));

	// 设置当前相机
	ForwardRenderer_->SetCurrentCamera(CameraActor_);

	int ShaderIndex = MShaderManager::GetInstance().LoadShader("Toon", "Toon/ToonVert.vert", "Toon/ToonFrag.frag");
	auto ToonShader = MShaderManager::GetInstance().GetShader(ShaderIndex);

	// 莱娜
	int LennaModelIndex = MModelManager::GetInstance().LoadModel("Lenna/Lenna.obj");
	auto LennaPtr = MModelManager::GetInstance().GetModel(LennaModelIndex);
	ModelActor_ = MySTL::TSharedPtr<AModelActor>(new AModelActor(LennaPtr, ToonShader));
	ModelActor_->SetActorWorldLocation(MyMath::FVector3(0, -2, 0));

	ModelActor_->SetDrawOutline(true);

	ForwardRenderer_->AddModel(ModelActor_);

	// 墙
	MyMath::Quaternion InitialQuat(MyMath::PI * (0.5), MyMath::FVector3(1, 0, 0));
	InitialQuat = MyMath::Quaternion(MyMath::PI / 2, MyMath::FVector3(0, 1, 0)) * InitialQuat;;
	for (int i = 0; i < 3; i++)
	{
		int WallModelIndex = MModelManager::GetInstance().LoadModel("wall/wall.obj");
		auto WallPtr = MModelManager::GetInstance().GetModel(WallModelIndex);
		WallActors_.emplace_back(new AModelActor(WallPtr, ToonShader));
		WallActors_[WallActors_.Size() - 1]->SetActorWorldRotation(InitialQuat);
		InitialQuat = MyMath::Quaternion(-MyMath::PI / 2, MyMath::FVector3(0, 1, 0)) * InitialQuat;
	}
	int WallModelIndex = MModelManager::GetInstance().LoadModel("wall/wall.obj");
	auto WallPtr = MModelManager::GetInstance().GetModel(WallModelIndex);
	WallActors_.emplace_back(new AModelActor(WallPtr, ToonShader));
	WallActors_[WallActors_.Size() - 1]->Rotate(MyMath::Quaternion(MyMath::PI, MyMath::FVector3(1, 0, 0)));
	WallActors_.emplace_back(new AModelActor(WallPtr, ToonShader));

	WallActors_[0]->SetActorWorldLocation(MyMath::FVector3(-4, 2, 0));
	WallActors_[0]->SetActorWorldScale(MyMath::FVector3(4.1, 4.1, 4.1));
	WallActors_[1]->SetActorWorldLocation(MyMath::FVector3(0, 2, -4));
	WallActors_[1]->SetActorWorldScale(MyMath::FVector3(4.1, 4.1, 4.1));
	WallActors_[2]->SetActorWorldLocation(MyMath::FVector3(4, 2, 0));
	WallActors_[2]->SetActorWorldScale(MyMath::FVector3(4.1, 4.1, 4.1));
	WallActors_[3]->SetActorWorldLocation(MyMath::FVector3(0, 6, 0));
	WallActors_[3]->SetActorWorldScale(MyMath::FVector3(4.1, 4.1, 4.1));
	WallActors_[4]->SetActorWorldLocation(MyMath::FVector3(0, -2, 0));
	WallActors_[4]->SetActorWorldScale(MyMath::FVector3(4.1, 4.1, 4.1));

	for (int i = 0; i < WallActors_.Size(); i++)
		ForwardRenderer_->AddModel(WallActors_[i]);

	//////// END TEST ////////
}

void FScene::Unload()
{
	// 关闭渲染管线
	ForwardRenderer_->Shutdown();
}

void FScene::Tick(float DeltaTime)
{
	CameraActor_->Tick(DeltaTime);

	auto Location = PointLightActor_->GetActorWorldLocation();
	Location = MyMath::Quaternion(DeltaTime * 0.5f, MyMath::FVector3(-0.5, 1, 0)) * Location;
	PointLightActor_->SetActorWorldLocation(Location);
}

void FScene::Render() 
{
	// 前向渲染
	ForwardRenderer_->RenderScene();
}
