// CameraActor.cpp
// 相机Actor
// created by FirstRay2001, Oct/12/2025

#include "CameraActor.h"
#include "MeshActor.h"
#include "PointLightActor.h"
#include "Source/Manager/InputManager.h"

ACameraActor::ACameraActor(float FovDegree, float Aspect, float Near, float Far) :
    FovDegree_(FovDegree),
    Aspect_(Aspect),
    Near_(Near),
    Far_(Far),
	MoveSpeed_(5.0f)
{
}

void ACameraActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);


	MyMath::FVector3 Forward = ActorTransform_.GetAbsoluteRotation().GetForwardVector();
	MyMath::FVector3 Right = ActorTransform_.GetAbsoluteRotation().GetRightVector();

	// WASD移动
	if (MInputManager::GetInstance().IsKeyPressed(GLFW_KEY_W))
	{
		MyMath::FVector3 Location = ActorTransform_.GetAbsoluteLocation();
		Location += Forward * DeltaTime * MoveSpeed_;
        SetActorWorldLocation(Location);
	}
	if (MInputManager::GetInstance().IsKeyPressed(GLFW_KEY_S))
	{
		MyMath::FVector3 Location = ActorTransform_.GetAbsoluteLocation();
		Location += Forward * DeltaTime * MoveSpeed_ * -1;
		SetActorWorldLocation(Location);
	}
	if (MInputManager::GetInstance().IsKeyPressed(GLFW_KEY_A))
	{
		MyMath::FVector3 Location = ActorTransform_.GetAbsoluteLocation();
		Location += Right * DeltaTime * MoveSpeed_ * -1;
		SetActorWorldLocation(Location);
	}
	if (MInputManager::GetInstance().IsKeyPressed(GLFW_KEY_D))
	{
		MyMath::FVector3 Location = ActorTransform_.GetAbsoluteLocation();
		Location += Right * DeltaTime * MoveSpeed_;
		SetActorWorldLocation(Location);
	}

	float OffsetX = MInputManager::GetInstance().GetMouseOffset()[0];
	float OffsetY = MInputManager::GetInstance().GetMouseOffset()[1];

	static bool FirstOffset = true;

	// 旋转视角
	if (abs(OffsetX) > MyMath::Epsilon || abs(OffsetY) > MyMath::Epsilon)
	{
		// 防止第一次偏移过大
		if (FirstOffset)
		{
			FirstOffset = false;
		}
		else
		{
			MyMath::Quaternion Rotation = ActorTransform_.GetAbsoluteRotation().GetQuaternion();
			MyMath::FVector3 Eular = Rotation.ToEular();

			float AngleX = -OffsetX * 0.1f * MyMath::Deg2Rad;
			float AngleY = OffsetY * 0.1f * MyMath::Deg2Rad;

			// 限制Pitch范围
			if (abs(Eular[0] * MyMath::Rad2Deg + AngleY * MyMath::Rad2Deg) > 89.0f)
			{
				AngleY = 0.0f;
			}

			// Pitch
			MyMath::Quaternion QuatPitch = MyMath::Quaternion(AngleY, Right);
			Rotate(QuatPitch);

			// Yaw
			MyMath::FVector3 Up = MyMath::FVector3(0, 1, 0);
			MyMath::Quaternion QuatYaw = MyMath::Quaternion(AngleX, Up);
			Rotate(QuatYaw);
		}
	}

	// 缩放视野
	float ScrollOffset = MInputManager::GetInstance().GetScrollOffset();
	if (abs(ScrollOffset) > MyMath::Epsilon)
	{
		FovDegree_ -= ScrollOffset;
		if (FovDegree_ < 1.0f)
		{
			FovDegree_ = 1.0f;
		}
		if (FovDegree_ > 90.0f)
		{
			FovDegree_ = 90.0f;
		}
	}

	// 缓存矩阵
	ViewMatrix_ = GetWorldMatrix().Inverse();
	ProjectionMatrix_ = GetProjectionMatrix();
}

MyMath::FMatrix ACameraActor::GetProjectionMatrix() const
{
	return MyMath::GetPerspectiveProjection(FovDegree_, Aspect_, Near_, Far_);
}

void ACameraActor::RenderMesh(AMeshActor* MeshActor) const
{
	// 设置相机位置
	MeshActor->SetCameraPos(GetActorWorldLocation());

    // 设置MVP矩阵
    MeshActor->SetModelMatrix();
    MeshActor->SetViewMatrix(ViewMatrix_);
    MeshActor->SetProjectionMatrix(ProjectionMatrix_);

    // 绘制
    MeshActor->Draw();
}

void ACameraActor::RenderPointLight(APointLightActor* PointLight) const
{
	// 设置MVP矩阵
	PointLight->SetModelMatrix();
	PointLight->SetViewMatrix(ViewMatrix_);
	PointLight->SetProjectionMatrix(ProjectionMatrix_);

	PointLight->SetCameraPos(GetActorWorldLocation());

	// 绘制
	PointLight->Draw();
}
