// Quaternion.h
// 四元数
// created by FirstRay2001, Sep/17/2025

#pragma once

#include "MathCommon.h"

namespace MyMath
{
	// 四元数
	class Quaternion
	{
	public:
		Quaternion(float W = 0, float X = 0, float Y = 0, float Z = 0) :
			WXYZ_(W, X, Y, Z)
		{
		}

		Quaternion(const FVector4& WXYZ) :
			WXYZ_(WXYZ)
		{

		}

		// 欧拉(弧度)构造四元数
		Quaternion(const FVector3& Eular) :
			WXYZ_()
		{
			float CosPitch = cos(Eular[0] * 0.5f);
			float SinPitch = sin(Eular[0] * 0.5f);
			float CosYaw = cos(Eular[1] * 0.5f);
			float SinYaw = sin(Eular[1] * 0.5f);
			float CosRoll = cos(Eular[2] * 0.5f);
			float SinRoll = sin(Eular[2] * 0.5f);
			W_ = CosRoll * CosPitch * CosYaw + SinRoll * SinPitch * SinYaw;
			X_ = SinRoll * CosPitch * CosYaw - CosRoll * SinPitch * SinYaw;
			Y_ = CosRoll * SinPitch * CosYaw + SinRoll * CosPitch * SinYaw;
			Z_ = CosRoll * CosPitch * SinYaw - SinRoll * SinPitch * CosYaw;
		}

		// 从弧度、转轴构建四元数
		Quaternion(const float Radian, const FVector3& Axis) :
			WXYZ_()
		{
			float SinValue = sin(Radian * 0.5f);
			float CosValue = cos(Radian * 0.5f);
			Real_ = CosValue;
			Image_ = Axis * SinValue;
		}

		Quaternion operator*(const Quaternion& Other) const
		{
			return{
				W_ * Other.W_ - X_ * Other.X_ - Y_ * Other.Y_ - Z_ * Other.Z_,
				W_ * Other.X_ + X_ * Other.W_ + Y_ * Other.Z_ - Z_ * Other.Y_,
				W_ * Other.Y_ - X_ * Other.Z_ + Y_ * Other.W_ + Z_ * Other.X_,
				W_ * Other.Z_ + X_ * Other.Y_ - Y_ * Other.X_ + Z_ * Other.W_
			};
		}

		// 共轭
		Quaternion Conjugate() const
		{
			return { W_, -X_, -Y_, -Z_ };
		}

		// 归一化
		Quaternion Normalized() const
		{
			float NormSqr = WXYZ_.NormalSqr();
			if (NormSqr < Epsilon)
				return { 1,0,0,0 };
			return { WXYZ_ * (1 / NormSqr) };
		}

		void Debug() const
		{
			printf("[Quaternion] ");
			WXYZ_.Debug();
		}

	private:
		union {
			FVector4 WXYZ_;
			struct
			{
				float Real_;
				FVector3 Image_;
			};
			struct
			{
				float W_;
				float X_;
				float Y_;
				float Z_;
			};
		};
	};
}
