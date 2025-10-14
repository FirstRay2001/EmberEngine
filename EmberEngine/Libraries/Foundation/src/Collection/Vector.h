// Vector.h
// ��̬����
// created by FirstRay2001, Sep/15/2025

#pragma once

#include <memory>

namespace MySTL
{
template<typename T>
class TVector
{
public:
	TVector() :
		Data_(nullptr),
		Capacity_(0),
		Size_(0)
	{
	}

	// ͨ��������Ĭ��ֵ����Vector
	explicit TVector(size_t Num, const T& Value = T()) :
		Data_(new T[Num]),
		Capacity_(Num),
		Size_(Num)
	{
		for (size_t i = 0; i < Size_; ++i)
			Data_[i] = Value;
	}

	// ��������
	TVector(const TVector<T>& Other) :
		Data_(new T[Other.Capacity_]),
		Capacity_(Other.Capacity_),
		Size_(Other.Size_)
	{
		// ���
		std::copy(Other.Data_, Other.Data_ + Size_, Data_);
	}

	// ������ֵ�����
	TVector& operator=(const TVector<T>& Other)
	{
		if (this != &Other)
		{
			delete Data_;
			Capacity_ = Other.Capacity_;
			Size_ = Other.Size_;
			Data_ = new T[Capacity_];

			// ���
			std::copy(Other.Data_, Other.Data_ + Size_, Data_);
		}
		return *this;
	}

	// �ƶ�����
	TVector(TVector<T>&& Other) noexcept :
		Data_(Other.Data_),
		Capacity_(Other.Capacity_),
		Size_(Other.Size_)
	{
		Other.Data_ = nullptr;
		Other.Capacity_ = 0;
		Other.Size_ = 0;
	}

	// �ƶ���ֵ�����
	TVector& operator=(TVector<T>&& Other) noexcept
	{
		if (this != &Other)
		{
			delete[] Data_;	// �������ͷ�֮ǰ���ڴ�
			Data_ = Other.Data_;
			Capacity_ = Other.Capacity_;
			Size_ = Other.Size_;
			Other.Data_ = nullptr;
			Other.Capacity_ = 0;
			Other.Size_ = 0;
		}
		return *this;
	}

	// ��������
	~TVector()
	{
		delete[] Data_;
	}

public:
	T&			operator[](size_t Index)		{ return Data_[Index]; }
	const T&	operator[](size_t Index) const	{ return Data_[Index]; }
	size_t		Size() const					{ return Size_; }
	size_t		Capacity() const				{ return Capacity_; }
	bool		Empty()	const					{ return Size_ == 0; }

	void Reserve(size_t NewCapacity)
	{
		if (NewCapacity > Capacity_)
			Reallocate(NewCapacity);
	}

	void Shrink()
	{
		Reallocate(Size_);
	}

	void Clear()
	{
		Size_ = 0;
	}

	void push_back(const T& Value)
	{
		if (Size_ >= Capacity_)
			Reserve(Capacity_ == 0 ? 1 : 2 * Capacity_);
		Data_[Size_++] = Value;
	}

	template<typename... Args>
	void emplace_back(Args&&... args)
	{
		if(Size_ >= Capacity_)
			Reserve(Capacity_ == 0 ? 1 : 2 * Capacity_);
		new(&Data_[Size_++]) T(std::forward<Args>(args)...);
	}

	void pop_back()
	{
		if (Size_ > 0)
			--Size_;
	}

	T* GetRawData()
	{
		return Data_;
	}

private:
	void Reallocate(size_t NewCapacity)
	{
		// �������ڴ�
		T* NewData = new T[NewCapacity];

		// ��������
		for (size_t i = 0; i < Size_ && i < NewCapacity; i++)
			NewData[i] = Data_[i];

		// �ͷž��ڴ�
		delete[] Data_;

		// ����ָ�������
		Data_ = NewData;
		Capacity_ = NewCapacity;
	}

private:
	T*		Data_;
	size_t	Capacity_;
	size_t	Size_;
};
}


