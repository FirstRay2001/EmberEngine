// CameraActor.h
// Ïà»úActor
// created by FirstRay2001, Oct/12/2025

#pragma once

#include "Actor.h"
// #include "Source/Graphics/Camera.h"

class ACameraActor : public AActor
{
public:
	ACameraActor(float Fov = 45.0f, float Aspect = 8.0f / 6.0f, float Near = 0.1f, float Far = 100.0f);

public:
	virtual void Tick(float DeltaTime) override;

public:
	MyMath::FMatrix GetProjectionMatrix() const;

	void RenderMesh(AMeshActor* MeshActor) const;

private:
	float FovDegree_;
	float Aspect_;
	float Near_;
	float Far_;
	float MoveSpeed_;
	MyMath::FMatrix ViewMatrix_;
	MyMath::FMatrix ProjectionMatrix_;
};
