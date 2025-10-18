// Camera.h
// œ‡ª˙¿‡
// created by FirstRay2001, Oct/16/2025

#pragma once

#include "Common.h"

class FCamera
{
public:
	FCamera(float Fov = 45.0f, float Aspect = 8.0f / 6.0f, float Near = 0.1f, float Far = 100.0f);
	~FCamera() {};

private:
	float FovDegree_;
	float Aspect_;
	float Near_;
	float Far_;
	float MoveSpeed_;
	MyMath::FMatrix ViewMatrix_;
	MyMath::FMatrix ProjectionMatrix_;
};
