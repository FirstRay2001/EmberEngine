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
class FVector2 : public FVector<float, 2>
{
public:
	explicit FVector2(float x = 0, float y = 0)
	{
		Data_[0] = x;
		Data_[1] = y;
	}

	FVector2(const FVector<float, 2>& Other) :
		FVector<float, 2>(Other)
	{
	}

	FVector2& operator=(const FVector<float, 2>& Other)
	{
		FVector<float, 2>::operator=(Other);
		return *this;
	}
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

	FVector3 operator*(const float& Value) const
	{
		return { Data_[0] * Value, Data_[1] * Value, Data_[2] * Value };
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

	// 模平方
	const float& NormalSqr()const
	{
		return (*this) * (*this);
	}

	void Debug() const
	{
		printf("[ %f, %f, %f ]", Data_[0], Data_[1], Data_[2]);
	}

private:
	float Data_[3];
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

private:
	float Data_[4];
};

// 四维矩阵
class FMatrix
{
public:
	FMatrix()
	{
		Identity();
	}

	float* GetRawData()
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

	// 矩阵数乘
	FMatrix operator*(const float& Value) const
	{
		FMatrix Ret;
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
}