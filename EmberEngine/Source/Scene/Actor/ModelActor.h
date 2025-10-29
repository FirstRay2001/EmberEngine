// ModelActor.h
// 模型Actor类
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

	void SetLightSpaceMatrix(const MyMath::FMatrix& LightSpacecMatrix) const;
	void SetBoneMatricesToShader(const MySTL::TWeakPtr<FShader>& Shader) const;

	void Draw() const;

	void RawDraw() const;

	const FShader* GetShader() const;

	bool IsDrawOutline() const { return bDrawOutline_; }
	void SetDrawOutline(bool bInDrawOutline) { bDrawOutline_ = bInDrawOutline; }
	bool IsDrawNormal() const { return bDrawNormal_; }
	void SetDrawNormal(bool bInDrawNormal) { bDrawNormal_ = bInDrawNormal; }
	void SetDrawBone(bool bDebug);

	void SetSkeleton(MySTL::TSharedPtr<FSkeleton> NewSkeleton);

private:
	MySTL::TWeakPtr<FModel>			Model_;
	MySTL::TWeakPtr<FShader>		Shader_;
	MySTL::TWeakPtr<FSkeleton>		Skeleton_;

	// 轮廓绘制开关
	bool bDrawOutline_;

	// 法线绘制开关
	bool bDrawNormal_;
 };
