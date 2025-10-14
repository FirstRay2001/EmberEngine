// Scene.cpp
// ≥°æ∞¿‡
// created by FirstRay2001, Sep/15/2025

#include "Scene.h"
#include "Source/Graphics/Mesh.h"
#include "Source/Graphics/Shader.h"
#include "Source/Scene/Actor/MeshActor.h"
#include "Source/Scene/Actor/CameraActor.h"
#include "Source/Graphics/Texture.h"

FScene::FScene()
{
}

void FScene::Load()
{

	// test
	/*FVertex
		ver1{ MyMath::FVector3(-0.5, -0.5, -0.5), MyMath::FVector3(1,1,1), MyMath::FVector2(0,0) },
		ver2{ MyMath::FVector3(0.5, -0.5, -0.5), MyMath::FVector3(1,1,1), MyMath::FVector2(1,0) },
		ver3{ MyMath::FVector3(0.5,  0.5, -0.5), MyMath::FVector3(1,1,1), MyMath::FVector2(1,1) },
		ver4{ MyMath::FVector3(-0.5,  0.5, -0.5), MyMath::FVector3(1,1,1), MyMath::FVector2(0,1) },
		ver5{ MyMath::FVector3(-0.5, -0.5, 0.5), MyMath::FVector3(1,1,1), MyMath::FVector2(1,0) },
		ver6{ MyMath::FVector3(0.5, -0.5, 0.5), MyMath::FVector3(1,1,1), MyMath::FVector2(0,0) },
		ver7{ MyMath::FVector3(0.5,  0.5, 0.5), MyMath::FVector3(1,1,1), MyMath::FVector2(0,1) },
		ver8{ MyMath::FVector3(-0.5,  0.5, 0.5), MyMath::FVector3(1,1,1), MyMath::FVector2(1,1) };
	MySTL::TVector<FVertex> Vertices;
	Vertices.push_back(ver1);
	Vertices.push_back(ver2);
	Vertices.push_back(ver3);
	Vertices.push_back(ver4);
	Vertices.push_back(ver5);
	Vertices.push_back(ver6);
	Vertices.push_back(ver7);
	Vertices.push_back(ver8);*/

	MySTL::TVector<FVertex> Vertices;
	FMesh::CreateVertex(Vertices, "D:/Dev/Project/EmberEngine/EmberEngine/Resources/Text/VertexData.txt");

	MySTL::TVector<unsigned int> Indices;
	int IndicesCnt = 0;
	MySTL::TUniquePtr<int> TempData(MyFS::ReadIntFromFile("D:/Dev/Project/EmberEngine/EmberEngine/Resources/Text/IndexData.txt", IndicesCnt));
	for (int i = 0; i < IndicesCnt; i++)
		Indices.push_back(TempData.Get()[i]);

	FMesh Mesh(Vertices, Indices);
	const char* VertexPath = "D:/Dev/Project/EmberEngine/EmberEngine/Resources/Shader/BlinnPhong/PhongVert.vert";
	const char* FragmentPath = "D:/Dev/Project/EmberEngine/EmberEngine/Resources/Shader/BlinnPhong/PhongFrag.frag";
	FShader Shader(VertexPath, FragmentPath);
	MeshActor_ = MySTL::TUniquePtr<AMeshActor>(new AMeshActor(Mesh, Shader));
	MeshActor_->SetActorWorldLocation(MyMath::FVector3(-0.5f, -0.2f, 0));
	MeshActor_->SetActorWorldRotation(FRotator(float(-20 * MyMath::Deg2Rad), MyMath::FVector3(0, 1, 0)));
	MeshActor_->SetActorWorldScale(MyMath::FVector3(1, 1, 1));

	const char* LightVertexPath = "D:/Dev/Project/EmberEngine/EmberEngine/Resources/Shader/LightBall/LightBallVert.vert";
	const char* LightFragmentPath = "D:/Dev/Project/EmberEngine/EmberEngine/Resources/Shader/LightBall/LightBallFrag.frag";
	FShader LightShader(LightVertexPath, LightFragmentPath);
	LightActor_ = MySTL::TUniquePtr<AMeshActor>(new AMeshActor(Mesh, LightShader));
	LightActor_->SetActorWorldLocation(MyMath::FVector3(1, 1, 2));
	LightActor_->SetActorWorldScale(MyMath::FVector3(0.2, 0.2, 0.2));

	CameraActor_ = MySTL::TUniquePtr<ACameraActor>(new ACameraActor());
	CameraActor_->SetActorWorldLocation(MyMath::FVector3(0, 0, 5));
	CameraActor_->SetActorWorldRotation(FRotator(float(0 * MyMath::Deg2Rad), MyMath::FVector3(0, 1, 0)));

	const char* PicPath = "D:/Dev/Project/EmberEngine/EmberEngine/Resources/Texture/awesomeface.png";
	FTexture tex(PicPath);

	MeshActor_->SetObjectColor(MyMath::FVector3(0.8f, 0, 0));
	MeshActor_->SetLightColor(MyMath::FVector3(0.5f, 0.5f, 0.5f));
}

void FScene::Unload()
{
}

void FScene::Tick(float DeltaTime)
{
	CameraActor_->Tick(DeltaTime);
	MeshActor_->Tick(DeltaTime);

	MyMath::FVector3 LightPos = LightActor_->GetActorWorldLocation();
	MyMath::Quaternion Quat(-0.1f * DeltaTime, MyMath::FVector3(0, 1, 1));
	LightPos = Quat * LightPos;
	// LightActor_->SetActorWorldLocation(LightPos);
}

void FScene::Render()
{
	MeshActor_->SetLightPos(LightActor_->GetActorWorldLocation());
	MeshActor_->SetCameraPos(CameraActor_->GetActorWorldLocation());
	CameraActor_->RenderMesh(MeshActor_.Get());
	CameraActor_->RenderMesh(LightActor_.Get());
}
