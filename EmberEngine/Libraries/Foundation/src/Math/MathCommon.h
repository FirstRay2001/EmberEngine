// MathCommon.h
// 数学库通用头文件
// created by FirstRay2001, Sep/15/2025

#pragma once

#include <iostream>

namespace MyMath
{
	const double PI = 3.14159265358979323846;
	const double Rad2Deg = 180.0 / PI;
	const double Deg2Rad = PI / 180.0;
	const double Epsilon = 2.2204460492503131e-016;

// 向量
template<typename T, size_t N>
class FVector
{
public:
	FVector() :
		Data_()
	{
	}

	// 取反
	FVector operator-() const
	{
		FVector<T, N> Ret;
		for (size_t i = 0; i < N; i++)
			Ret.Data_[i] = -1 * Data_[i];
		return Ret;
	}

	// 标量加
	FVector& operator+= (const T& Value)
	{
		for (size_t i = 0; i < N; i++)
			Data_[i] += Value;
		return *this;
	}

	// 标量减
	FVector& operator-= (const T& Value)
	{
		(*this) += (-1) * Value;
		return *this;
	}

	// 标量乘
	FVector operator*(const T& Value)
	{
		FVector<T, N> Ret;
		for (size_t i = 0; i < N;i++)
			Ret.Data_[i] *= Value;
		return Ret;
	}

	// 标量除
	FVector operator/(const T& Value)
	{
		return (*this) * (1 / Value);
	}

	// 向量加
	FVector& operator+= (const FVector<T, N>& Other)
	{
		for (size_t i = 0; i < N; i++)
			Data_[i] += Other.Data_[i];
		return *this;
	}

	FVector operator+(const FVector<T, N>& Other) const
	{
		FVector<T, N> Ret;
		for (size_t i = 0; i < N;i++)
			Ret.Data_[i] = Data_[i] + Other.Data_[i];
		return Ret;
	}
	
	// 向量减
	FVector& operator-=(const FVector<T, N>& Other)
	{
		return (*this) += -Other;
	}

	FVector operator-(const FVector<T, N>& Other)
	{
		return (*this) + (-Other);
	}

	T& operator[](size_t Index)
	{
		return Data_[Index];
	}

	const T& operator[](size_t Index) const
	{
		return Data_[Index];
	}

	float* GetRawData()
	{
		return Data_;
	}

	void Debug()
	{
		std::cout << "[ ";
		for (size_t i = 0; i < N; i++)
			std::cout << Data_[i] <<" ";
		std::cout << "]" << std::endl;
	}

protected:
	float Data_[N];
};

// 二维向量
class FVector2
{
public:
	FVector2(float x = 0, float y = 0)
	{
		Data_[0] = x;
		Data_[1] = y;
	}

	FVector2 operator+(const FVector2& Other) const
	{
		return { Data_[0] + Other.Data_[0], Data_[1] + Other.Data_[1]};
	}

	FVector2 operator*(const float& Value) const
	{
		return { Data_[0] * Value, Data_[1] * Value };
	}

	float operator*(const FVector2& Other) const
	{
		return Data_[0] * Other.Data_[0] + Data_[1] * Other.Data_[1];
	}

	float& operator[](size_t Index)
	{
		return Data_[Index];
	}

	const float& operator[](size_t Index) const
	{
		return Data_[Index];
	}

	// 模平方
	const float& NormalSqr() const
	{
		return (*this) * (*this);
	}

	void Debug() const
	{
		printf("[ %f, %f ]", Data_[0], Data_[1]);
	}

private:
	float Data_[2];
};

// 三维向量
class FVector3
{
public:
	FVector3(float x = 0, float y = 0, float z = 0) :
		Data_()
	{
		Data_[0] = x;
		Data_[1] = y;
		Data_[2] = z;
	}

	FVector3 operator+(const FVector3& Other) const
	{
		return { Data_[0] + Other.Data_[0], Data_[1] + Other.Data_[1], Data_[2] + Other.Data_[2] };
	}

	FVector3 operator-(const FVector3& Other) const
	{
		return { Data_[0] - Other.Data_[0], Data_[1] - Other.Data_[1], Data_[2] - Other.Data_[2] };
	}

	FVector3& operator+=(const FVector3& Other)
	{
		Data_[0] += Other.Data_[0];
		Data_[1] += Other.Data_[1];
		Data_[2] += Other.Data_[2];
		return *this;
	}

	FVector3 operator*(const float& Value) const
	{
		return { Data_[0] * Value, Data_[1] * Value, Data_[2] * Value };
	}

	FVector3& operator*=(const float& Value)
	{
		Data_[0] *= Value;
		Data_[1] *= Value;
		Data_[2] *= Value;
		return *this;
	}

	float operator*(const FVector3& Other) const
	{
		return Data_[0] * Other.Data_[0] + Data_[1] * Other.Data_[1] + Data_[2] * Other.Data_[2];
	}

	float& operator[](size_t Index)
	{
		return Data_[Index];
	}

	const float& operator[](size_t Index) const
	{
		return Data_[Index];
	}

	FVector3 Normalized() const
	{
		float Sqr = NormalSqr();

		if (Sqr < Epsilon)
			return *this;

		return (*this) * (1.0f / sqrt(Sqr));
	}

	// 模平方
	const float& NormalSqr() const
	{
		return (*this) * (*this);
	}

	void Debug() const
	{
		printf("[ %f, %f, %f ]", Data_[0], Data_[1], Data_[2]);
	}

public:
	union 
	{
		float Data_[3];
		struct { float X, Y, Z; };
	};
};

// 四维向量
class FVector4
{
public:
	FVector4(float x = 0, float y = 0, float z = 0, float w = 0) :
		Data_()
	{
		Data_[0] = x;
		Data_[1] = y;
		Data_[2] = z;
		Data_[3] = w;
	}

	FVector4 operator+(const FVector4& Other) const
	{
		return { Data_[0] + Other.Data_[0], Data_[1] + Other.Data_[1], Data_[2] + Other.Data_[2], Data_[3] + Other.Data_[3] };
	}

	FVector4 operator*(const float& Value) const
	{
		return { Data_[0] * Value, Data_[1] * Value, Data_[2] * Value, Data_[3] * Value };
	}

	float operator*(const FVector4& Other) const
	{
		return Data_[0] * Other.Data_[0] + Data_[1] * Other.Data_[1] + Data_[2] * Other.Data_[2] + Data_[3] * Other.Data_[3];
	}

	float& operator[](size_t Index)
	{
		return Data_[Index];
	}

	const float& operator[](size_t Index) const
	{
		return Data_[Index];
	}

	// 模平方
	const float& NormalSqr()const
	{
		return (*this) * (*this);
	}

	void Debug() const
	{
		printf("[ %f, %f, %f, %f ]", Data_[0], Data_[1], Data_[2], Data_[3]);
	}

public:
	union {
		float Data_[4];
		struct
		{
			float X, Y, Z, W;
		};
	};
};

// 四维矩阵
class FMatrix
{
public:
	FMatrix()
	{
		Identity();
	}

	FMatrix(float a1, float a2, float a3, float a4,
		float b1, float b2, float b3, float b4,
		float c1, float c2, float c3, float c4,
		float d1, float d2, float d3, float d4) :
		Data_()
	{
		Data_[0] = a1;
		Data_[1] = a2;
		Data_[2] = a3;
		Data_[3] = a4;
		Data_[4] = b1;
		Data_[5] = b2;
		Data_[6] = b3;
		Data_[7] = b4;
		Data_[8] = c1;
		Data_[9] = c2;
		Data_[10] = c3;
		Data_[11] = c4;
		Data_[12] = d1;
		Data_[13] = d2;
		Data_[14] = d3;
		Data_[15] = d4;
	}

	// 赋值运算
	FMatrix& operator=(const FMatrix& Other)
	{
		if (this != &Other)
			memcpy(Data_, Other.Data_, 16 * sizeof(float));
		return *this;
	}

	const float* GetRawData() const
	{
		return Data_;
	}

	float& At(size_t Row, size_t Col)
	{
		size_t Index = Row * 4 + Col;
		if (Index < 0 || Index >= 16)
		{
			std::cout << "Matrix index out of range" << std::endl;
			return Data_[0];
		}
		return Data_[Index];
	}

	const float& At(size_t Row, size_t Col) const
	{
		size_t Index = Row * 4 + Col;
		if (Index < 0 || Index >= 16)
		{
			std::cout << "Matrix index out of range" << std::endl;
			return Data_[0];
		}
		return Data_[Index];
	}

	float& operator()(size_t Row, size_t Col)
	{
		return At(Row, Col);
	}

	const float& operator()(size_t Row, size_t Col) const
	{
		return At(Row, Col);
	}

	// 转化为单位矩阵
	void Identity()
	{
		Zero();
		for (size_t i = 0; i < 4; i++)
			At(i, i) = 1;
	}

	// 矩阵置零
	void Zero()
	{
		memset(Data_, 0, 16 * sizeof(float));
	}

	// 矩阵转置
	FMatrix& Transpose()
	{
		float Temp[16];
		memcpy(Temp, Data_, 16 * sizeof(float));
		for (size_t i = 0; i < 4;i++)
			for (size_t j = 0; j < 4;j++)
				Data_[j * 4 + i] = Temp[i * 4 + j];
		return *this;
	}

	FMatrix TransposeConst() const
	{
		float Temp[16];
		memcpy(Temp, Data_, 16 * sizeof(float));
		FMatrix Ret;
		for (size_t i = 0; i < 4; i++)
			for (size_t j = 0; j < 4; j++)
				Ret.Data_[j * 4 + i] = Temp[i * 4 + j];
		return Ret;
	}

	// 矩阵数乘
	FMatrix operator*(const float& Value) const
	{
		FMatrix Ret = *this;
		for (size_t i = 0; i < 16; i++)
			Ret.Data_[i] *= Value;
		return Ret;
	}

	FMatrix& operator*=(const float& Value)
	{
		for (size_t i = 0; i < 16; i++)
			Data_[i] *= Value;
		return *this;
	}

	// 矩阵乘四维向量
	FVector4 operator*(const FVector4& Vec) const
	{
		FVector4 Ret;
		for (size_t i = 0; i < 4; i++)
		{
			float Sum = 0.0f;
			for (size_t j = 0; j < 4; j++)
				Sum += At(i, j) * Vec[j];
			Ret[i] = Sum;
		}
		return Ret;
	}

	// 矩阵乘三维向量
	FVector3 operator*(const FVector3& Vec) const
	{
		FVector3 Ret;
		for (size_t i = 0; i < 3; i++)
		{
			float Sum = 0.0f;
			for (size_t j = 0; j < 3; j++)
				Sum += At(i, j) * Vec[j];
			Ret[i] = Sum;
		}
		return Ret;
	}

	// 矩阵加法
	FMatrix operator+(const FMatrix& Other)
	{
		FMatrix Ret;
		for (size_t i = 0; i < 16; i++)
			Ret.Data_[i] = Data_[i] + Other.Data_[i];
		return Ret;
	}

	// 矩阵减法
	FMatrix operator-(const FMatrix& Other)
	{
		return *this + Other * (-1.0f);
	}

	// 矩阵乘法
	FMatrix operator*(const FMatrix& Other) const
	{
		FMatrix Ret;
		Ret.Zero();
		for (size_t i = 0; i < 4;i++)
			for (size_t j = 0; j < 4;j++)
				for (size_t k = 0; k < 4;k++)
					Ret(i, j) += At(i, k) * Other.At(k, j);
		return Ret;
	}

	// 三阶余子式
	float Determinant3x3(size_t row, size_t col) const
	{
		double SubMat[9];
		size_t Index = 0;

		// 获取子矩阵
		for (size_t i = 0; i < 4; i++)
		{
			if (i == row)
				continue;
			for (size_t j = 0; j < 4; j++)
			{
				if (j == col)
					continue;
				SubMat[Index++] = At(i, j);
			}
		}

		double a = SubMat[0], b = SubMat[1], c = SubMat[2],
			d = SubMat[3], e = SubMat[4], f = SubMat[5],
			g = SubMat[6], h = SubMat[7], i = SubMat[8];

		return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
	}

	// 四阶矩阵行列式
	float Determinant() const
	{
		return At(0, 0) * Determinant3x3(0, 0) - At(0, 1) * Determinant3x3(0, 1) + At(0, 2) * Determinant3x3(0, 2) - At(0, 3) * Determinant3x3(0, 3);
	}

	// 逆矩阵
	FMatrix Inverse() const
	{
		FMatrix InverseMat, Adjugate;

		// 行列式
		float Det = Determinant();

		// 行列式为零，矩阵不可逆
		if (fabs(Det) < Epsilon)
		{
			printf("matrix is not invertible\n");
			return InverseMat;
		}
		
		// 计算伴随矩阵
		Adjugate(0, 0) = Determinant3x3(0, 0);
		Adjugate(1, 0) = -Determinant3x3(0, 1);
		Adjugate(2, 0) = Determinant3x3(0, 2);
		Adjugate(3, 0) = -Determinant3x3(0, 3);
		Adjugate(0, 1) = -Determinant3x3(1, 0);
		Adjugate(1, 1) = Determinant3x3(1, 1);
		Adjugate(2, 1) = -Determinant3x3(1, 2);
		Adjugate(3, 1) = Determinant3x3(1, 3);
		Adjugate(0, 2) = Determinant3x3(2, 0);
		Adjugate(1, 2) = -Determinant3x3(2, 1);
		Adjugate(2, 2) = Determinant3x3(2, 2);
		Adjugate(3, 2) = -Determinant3x3(2, 3);
		Adjugate(0, 3) = -Determinant3x3(3, 0);
		Adjugate(1, 3) = Determinant3x3(3, 1);
		Adjugate(2, 3) = -Determinant3x3(3, 2);
		Adjugate(3, 3) = Determinant3x3(3, 3);

		// 计算逆矩阵
		InverseMat = Adjugate * (1 / Det);

		return InverseMat;
	}

	FVector3 GetTranslation() const
	{
		return FVector3(At(0, 3), At(1, 3), At(2, 3));
	}

	void Debug()
	{
		for (size_t i = 0; i < 4; i++)
		{
			std::cout << "[ ";
			for (size_t j = 0; j < 4; j++)
			{
				std::cout << Data_[i * 4 + j] << " ";
			}
			std::cout << "]" << std::endl;
		}
	}

private:
	float Data_[16];
};

// 获取位移矩阵
inline FMatrix ToTranslationMatrix(const FVector3& Translation)
{
	FMatrix Ret;
	Ret(0, 3) = Translation[0];
	Ret(1, 3) = Translation[1];
	Ret(2, 3) = Translation[2];
	return Ret;
}

// 获取缩放矩阵
inline FMatrix ToScaleMatrix(const FVector3& Scale)
{
	FMatrix Ret;
	Ret(0, 0) = Scale[0];
	Ret(1, 1) = Scale[1];
	Ret(2, 2) = Scale[2];
	return Ret;
}

// 向量叉乘
inline FVector3 Cross(const FVector3& A, const FVector3& B)
{
	return FVector3(
		A[1] * B[2] - A[2] * B[1],
		A[2] * B[0] - A[0] * B[2],
		A[0] * B[1] - A[1] * B[0]
	);
}

// 向量点乘
inline float Dot(const FVector3& A, const FVector3& B)
{
	return A[0] * B[0] + A[1] * B[1] + A[2] * B[2];
}

// 获取LookAt矩阵
inline FMatrix GetLookAt(const FVector3& Eye, const FVector3& Target, const FVector3& Up)
{
	FVector3 ZAxis = (Eye - Target).Normalized();
	FVector3 XAxis = Cross(Up, ZAxis).Normalized();
	FVector3 YAxis = Cross(ZAxis, XAxis);
	FMatrix ViewMatrix;
	ViewMatrix(0, 0) = XAxis[0];
	ViewMatrix(0, 1) = XAxis[1];
	ViewMatrix(0, 2) = XAxis[2];
	ViewMatrix(0, 3) = -(XAxis * Eye);
	ViewMatrix(1, 0) = YAxis[0];
	ViewMatrix(1, 1) = YAxis[1];
	ViewMatrix(1, 2) = YAxis[2];
	ViewMatrix(1, 3) = -(YAxis * Eye);
	ViewMatrix(2, 0) = ZAxis[0];
	ViewMatrix(2, 1) = ZAxis[1];
	ViewMatrix(2, 2) = ZAxis[2];
	ViewMatrix(2, 3) = -(ZAxis * Eye);
	ViewMatrix(3, 0) = 0;
	ViewMatrix(3, 1) = 0;
	ViewMatrix(3, 2) = 0;
	ViewMatrix(3, 3) = 1;
	return ViewMatrix;
}

// 获取正交投影矩阵
inline FMatrix GetOrthoProjection(float Left, float Right, float Bottom, float Top, float Near, float Far)
{
	FMatrix ProjectionMatrix;
	ProjectionMatrix(0, 0) = 2.0f / (Right - Left);
	ProjectionMatrix(0, 1) = 0;
	ProjectionMatrix(0, 2) = 0;
	ProjectionMatrix(0, 3) = -(Right + Left) / (Right - Left);
	ProjectionMatrix(1, 0) = 0;
	ProjectionMatrix(1, 1) = 2.0f / (Top - Bottom);
	ProjectionMatrix(1, 2) = 0;
	ProjectionMatrix(1, 3) = -(Top + Bottom) / (Top - Bottom);
	ProjectionMatrix(2, 0) = 0;
	ProjectionMatrix(2, 1) = 0;
	ProjectionMatrix(2, 2) = -2.0f / (Far - Near);
	ProjectionMatrix(2, 3) = -(Far + Near) / (Far - Near);
	ProjectionMatrix(3, 0) = 0;
	ProjectionMatrix(3, 1) = 0;
	ProjectionMatrix(3, 2) = 0;
	ProjectionMatrix(3, 3) = 1;
	return ProjectionMatrix;
}

// 获取透视投影矩阵
inline FMatrix GetPerspectiveProjection(float FovDegree, float Aspect, float Near, float Far)
{
	float FovRad = FovDegree * Deg2Rad;
	float TanHalfFov = tan(FovRad / 2.0f);
	FMatrix ProjectionMatrix;
	ProjectionMatrix(0, 0) = 1.0f / (Aspect * TanHalfFov);
	ProjectionMatrix(0, 1) = 0;
	ProjectionMatrix(0, 2) = 0;
	ProjectionMatrix(0, 3) = 0;
	ProjectionMatrix(1, 0) = 0;
	ProjectionMatrix(1, 1) = 1.0f / (TanHalfFov);
	ProjectionMatrix(1, 2) = 0;
	ProjectionMatrix(1, 3) = 0;
	ProjectionMatrix(2, 0) = 0;
	ProjectionMatrix(2, 1) = 0;
	ProjectionMatrix(2, 2) = -(Far + Near) / (Far - Near);
	ProjectionMatrix(2, 3) = -(2.0f * Far * Near) / (Far - Near);
	ProjectionMatrix(3, 0) = 0;
	ProjectionMatrix(3, 1) = 0;
	ProjectionMatrix(3, 2) = -1.0f;
	ProjectionMatrix(3, 3) = 0;
	return ProjectionMatrix;
}

// 三维向量插值
inline FVector3 Lerp(const FVector3& A, const FVector3& B, float T)
{
	return A * (1.0f - T) + B * T;
}

} // namespace MyMath