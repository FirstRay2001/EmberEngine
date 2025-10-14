// Actor.h
// Actor��
// created by FirstRay2001, Sep/15/2025

#pragma once

#include "Common.h"
#include "Source/Scene/Transform.h"

class AActor
{
public:
	AActor(FTransform Transform = FTransform());
	virtual ~AActor() {};

	//// �������� ////
	virtual void BeginPlay()			{ };
	virtual void Tick(float DeltaTime)	{ };
	virtual void EndPlay()				{ };

	//// Transform������ ////

	void SetActorWorldLocation(const MyMath::FVector3& NewLocation);
	void SetActorWorldRotation(const FRotator& NewRotation);
	void SetActorWorldScale(const MyMath::FVector3& NewScale);

	MyMath::FVector3 GetActorWorldLocation() const;

protected:
	MyMath::FMatrix GetWorldMatrix() const;

protected:
	FTransform ActorTransform_;

private:

};