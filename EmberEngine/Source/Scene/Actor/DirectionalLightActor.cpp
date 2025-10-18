// DirectionalLightActor.cpp
// 平行光源类
// created by FirstRay2001, Oct/15/2025

#include "DirectionalLightActor.h"

ADirectionalLightActor::ADirectionalLightActor(const MyMath::FVector3& Direction) :
	Direction_(Direction.Normalized()),
	AmbientColor_(MyMath::FVector3(0.2f, 0.2f, 0.2f)),
	DiffuseColor_(MyMath::FVector3(1.0f, 1.0f, 1.0f)),
	SpecularColor_(MyMath::FVector3(1.0f, 1.0f, 1.0f))
{
}
