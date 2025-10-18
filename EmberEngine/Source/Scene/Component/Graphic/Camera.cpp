// Camera.cpp
// Ïà»úÀà
// created by FirstRay2001, Oct/16/2025

#include "Camera.h"

FCamera::FCamera(float Fov, float Aspect, float Near, float Far) :
	FovDegree_(Fov),
	Aspect_(Aspect),
	Near_(Near),
	Far_(Far),
	MoveSpeed_(2.5f),
	ViewMatrix_(),
	ProjectionMatrix_()
{
}
