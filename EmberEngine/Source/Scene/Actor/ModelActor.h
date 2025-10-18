// ModelActor.h
// Ä£ÐÍActorÀà
// created by FirstRay2001, Oct/16/2025

#pragma once

#include "Common.h"
#include "Source/Scene/Actor/Actor.h"

class AModelActor : public AActor
{
public:
	AModelActor(MySTL::TSharedPtr<FModel> Model, MySTL::TSharedPtr<FShader> Shader);

	// Dispose
	AModelActor(FMesh* Mesh, FMaterial* Material, FShader* Shader);
	virtual ~AModelActor() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void SetCamera(const ACameraActor& Camera) const;
	void SetPointLight(const APointLightActor& PointLight) const;
	void SetDirectionalLight(const ADirectionalLightActor& DirectionalLight) const;

	void Draw() const;

private:
	MySTL::TWeakPtr<FModel> Model_;
	MySTL::TWeakPtr<FShader> Shader_;
 };
