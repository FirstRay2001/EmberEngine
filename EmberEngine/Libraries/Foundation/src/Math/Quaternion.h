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
		Quaternion(float W = 0, float X = 0, float Y = 1, float Z = 0) :
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

		MyMath::FVector3 operator*(const MyMath::FVector3& Vec) const
		{
			// 计算四元数与向量的乘积，向量被视为纯虚四元数(实部为0)
			Quaternion VecQuat(0, Vec[0], Vec[1], Vec[2]);
			Quaternion ResultQuat = (*this) * VecQuat * this->Conjugate();
			return ResultQuat.Image_;
		}

		// 取消 *= 以避免左右乘失误
		/*Quaternion& operator*=(const Quaternion& Other)
		{
			*this = (*this) * Other;
			return *this;
		}*/

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
			return { WXYZ_ * (1 / sqrt(NormSqr)) };
		}

		// 获取旋转矩阵
		FMatrix ToMatrix() const
		{
			Quaternion NormalQuat = Normalized();

			float xx = NormalQuat.X_ * NormalQuat.X_;
			float xy = NormalQuat.X_ * NormalQuat.Y_;
			float xz = NormalQuat.X_ * NormalQuat.Z_;
			float xw = NormalQuat.X_ * NormalQuat.W_;
			float yy = NormalQuat.Y_ * NormalQuat.Y_;
			float yz = NormalQuat.Y_ * NormalQuat.Z_;
			float yw = NormalQuat.Y_ * NormalQuat.W_;
			float zz = NormalQuat.Z_ * NormalQuat.Z_;
			float zw = NormalQuat.Z_ * NormalQuat.W_;

			FMatrix Ret;

			Ret(0, 0) = 1 - 2 * (yy + zz);
			Ret(0, 1) = 2 * (xy - zw);
			Ret(0, 2) = 2 * (xz + yw);
			Ret(1, 0) = 2 * (xy + zw);
			Ret(1, 1) = 1 - 2 * (xx + zz);
			Ret(1, 2) = 2 * (yz - xw);
			Ret(2, 0) = 2 * (xz - yw);
			Ret(2, 1) = 2 * (yz + xw);
			Ret(2, 2) = 1 - 2 * (xx + yy);

			return Ret;
		}

		// 转为欧拉角(弧度)
		FVector3 ToEular() const
		{
			FVector3 Ret;

			// pitch
			float SinPitch = 2.0f * (W_ * X_ + Y_ * Z_);
			float CosPitch = 1.0f - 2.0f * (X_ * X_ + Y_ * Y_);
			Ret[0] = atan2(SinPitch, CosPitch);
			if (Ret[0] > 90.0f * Deg2Rad)
			{
				Ret[0] -= 180.0f * Deg2Rad;
			}
			else if (Ret[0] < -90.0f * Deg2Rad)
			{
				Ret[0] += 180.0f * Deg2Rad;
			}
			
			// yaw
			float SinYaw = 2.0f * (W_ * Y_ - Z_ * X_);
			if (fabs(SinYaw) >= 1)
				Ret[1] = copysign(PI / 2, SinYaw);
			else
				Ret[1] = asin(SinYaw);
			
			// roll
			float SinRoll = 2.0f * (W_ * Z_ + X_ * Y_);
			float CosRoll = 1.0f - 2.0f * (Y_ * Y_ + Z_ * Z_);
			Ret[2] = atan2(SinRoll, CosRoll);
			return Ret;
		}

		// 获取方向向量（初始方向为Z轴负方向）
		FVector3 GetForwardDirection() const
		{
			FVector3 Forward = { 0, 0, -1 };

			FVector3 NewForward = {
				(1 - 2 * (Y_ * Y_ + Z_ * Z_)) * Forward[0] + (2 * (X_ * Y_ - W_ * Z_)) * Forward[1] + (2 * (X_ * Z_ + W_ * Y_)) * Forward[2],
				(2 * (X_ * Y_ + W_ * Z_)) * Forward[0] + (1 - 2 * (X_ * X_ + Z_ * Z_)) * Forward[1] + (2 * (Y_ * Z_ - W_ * X_)) * Forward[2],
				(2 * (X_ * Z_ - W_ * Y_)) * Forward[0] + (2 * (Y_ * Z_ + W_ * X_)) * Forward[1] + (1 - 2 * (X_ * X_ + Y_ * Y_)) * Forward[2]
			};

			return NewForward.Normalized();
		}

		// 获取上方向向量（初始方向为Y轴正方向）
		FVector3 GetUpDirection() const
		{
			FVector3 Up = { 0,1,0 };
			FVector3 NewUp = {
				(1 - 2 * (Y_ * Y_ + Z_ * Z_)) * Up[0] + (2 * (X_ * Y_ - W_ * Z_)) * Up[1] + (2 * (X_ * Z_ + W_ * Y_)) * Up[2],
				(2 * (X_ * Y_ + W_ * Z_)) * Up[0] + (1 - 2 * (X_ * X_ + Z_ * Z_)) * Up[1] + (2 * (Y_ * Z_ - W_ * X_)) * Up[2],
				(2 * (X_ * Z_ - W_ * Y_)) * Up[0] + (2 * (Y_ * Z_ + W_ * X_)) * Up[1] + (1 - 2 * (X_ * X_ + Y_ * Y_)) * Up[2]
			};
			return NewUp.Normalized();
		}

		// 获取右方向向量（初始方向为X轴正方向）
		FVector3 GetRightDirection() const
		{
			FVector3 Right = { 1,0,0 };
			FVector3 NewRight = {
				(1 - 2 * (Y_ * Y_ + Z_ * Z_)) * Right[0] + (2 * (X_ * Y_ - W_ * Z_)) * Right[1] + (2 * (X_ * Z_ + W_ * Y_)) * Right[2],
				(2 * (X_ * Y_ + W_ * Z_)) * Right[0] + (1 - 2 * (X_ * X_ + Z_ * Z_)) * Right[1] + (2 * (Y_ * Z_ - W_ * X_)) * Right[2],
				(2 * (X_ * Z_ - W_ * Y_)) * Right[0] + (2 * (Y_ * Z_ + W_ * X_)) * Right[1] + (1 - 2 * (X_ * X_ + Y_ * Y_)) * Right[2]
			};
			return NewRight.Normalized();
		}

		void Debug() const
		{
			printf("[Quaternion] ");
			WXYZ_.Debug();
		}

	public:
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

	// 四元数插值
	inline Quaternion Slerp(const Quaternion& A, const Quaternion& B, float T)
	{
		// 计算点积
		float DotProduct = A.W_ * B.W_ + A.X_ * B.X_ + A.Y_ * B.Y_ + A.Z_ * B.Z_;

		// 如果点积为负，则反转B以确保最短路径插值
		Quaternion BAdjusted = B;
		if (DotProduct < 0.0f)
		{
			BAdjusted = Quaternion(-B.W_, -B.X_, -B.Y_, -B.Z_);
			DotProduct = -DotProduct;
		}

		// 如果点积接近1，使用线性插值以避免数值不稳定
		if (DotProduct > 0.9995f)
		{
			Quaternion Result = Quaternion(
				A.W_ + T * (BAdjusted.W_ - A.W_),
				A.X_ + T * (BAdjusted.X_ - A.X_),
				A.Y_ + T * (BAdjusted.Y_ - A.Y_),
				A.Z_ + T * (BAdjusted.Z_ - A.Z_)
			);
			return Result.Normalized();
		}

		// 计算角度和插值因子
		float Theta0 = acos(DotProduct);
		float SinTheta0 = sin(Theta0);
		float Theta = Theta0 * T;
		float SinTheta = sin(Theta);
		float S0 = cos(Theta) - DotProduct * SinTheta / SinTheta0;
		float S1 = SinTheta / SinTheta0;

		// 计算插值结果
		Quaternion Result = Quaternion(
			S0 * A.W_ + S1 * BAdjusted.W_,
			S0 * A.X_ + S1 * BAdjusted.X_,
			S0 * A.Y_ + S1 * BAdjusted.Y_,
			S0 * A.Z_ + S1 * BAdjusted.Z_
		);
		return Result;
	}
}
