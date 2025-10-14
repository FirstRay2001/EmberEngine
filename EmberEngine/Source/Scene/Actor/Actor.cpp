// GameObject.cpp
// Actor¿‡
// created by FirstRay2001, Sep/15/2025

#include "Actor.h"

AActor::AActor(FTransform Transform) :
	ActorTransform_(Transform)
{
}

void AActor::SetActorWorldLocation(const MyMath::FVector3& NewLocation)
{
	ActorTransform_.SetAbsoluteLocation(NewLocation);
}

void AActor::SetActorWorldRotation(const FRotator& NewRotation)
{
	ActorTransform_.SetAbsoluteRotation(NewRotation);
}

void AActor::SetActorWorldScale(const MyMath::FVector3& NewScale)
{
	ActorTransform_.SetAbsoluteScale(NewScale);
}

MyMath::FVector3 AActor::GetActorWorldLocation() const
{
	return ActorTransform_.GetAbsoluteLocation();
}

MyMath::FMatrix AActor::GetWorldMatrix() const
{
	return ActorTransform_.GetTransformMatrix();
}
