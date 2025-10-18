// SmartPointer.h
// ����ָ��
// created by FirstRay2001, Sep/15/2025

#pragma once

namespace MySTL
{
template<typename T>
class TUniquePtr
{
public:
	// ���ÿ�������
	TUniquePtr(TUniquePtr<T>& Other) = delete;

	// �ӹ�ԭʼָ��
	TUniquePtr(T* OriginalPtr = nullptr) :
		Ptr_(OriginalPtr)
	{
	}

	// �ƶ�����
	TUniquePtr(TUniquePtr<T>&& Other) noexcept :
		Ptr_(Other.Ptr_)
	{
		// �ͷ���һ��ָ����Դ
		Other.Ptr_ = nullptr;
	}

	// �ƶ���ֵ
	TUniquePtr<T>& operator=(TUniquePtr<T>&& Other) noexcept
	{
		// �����Ҹ�ֵ
		if (&Other != this)
		{
			// �ͷž���Դ
			delete Ptr_;

			// �ӹ�����Դ
			Ptr_ = Other.Ptr_;
			Other.Ptr_ = nullptr;
		}
		return *this;
	}

	// ����
	~TUniquePtr()
	{
		delete Ptr_;
	}

public:
	T& operator*()
	{
		return *Ptr_;
	}

	const T& operator*() const
	{
		return *Ptr_;
	}

	T* operator->()
	{
		return Ptr_;
	}

	const T* operator->() const
	{
		return Ptr_;
	}

	T* Get()
	{
		return Ptr_;
	}

	// �ͷ�����Ȩ������ԭʼָ��
	T* Release()
	{
		T* OldPtr = Ptr_;
		Ptr_ = nullptr;
		return OldPtr;
	}

	// ����ָ��
	void Reset(T* NewPtr = nullptr) noexcept
	{
		if (NewPtr == Ptr_) return;
		T* OldPtr = Ptr_;
		Ptr_ = NewPtr;
		delete OldPtr;
	}

	// ��Ч�Լ��
	explicit operator bool() const
	{
		return Ptr_ != nullptr;
	}

private:
	T* Ptr_;
};

// ��Ԫǰ������
template<typename T>
class TWeakPtr;

template<typename T>
class TSharedPtr
{
public:
	struct FCountBlock
	{
		size_t SharedCount;
		size_t WeakCount;
	};

	friend class TWeakPtr<T>;

public:
	// ��ʼ������
	explicit TSharedPtr(T* OriginalPtr = nullptr) :
		Ptr_(OriginalPtr),
		CntBlk_(OriginalPtr == nullptr ? nullptr : new FCountBlock{ 1, 0 })
	{
	}

	// ��������
	TSharedPtr(const TSharedPtr& Other) :
		Ptr_(Other.Ptr_),
		CntBlk_(Other.CntBlk_)
	{
		if (CntBlk_ != nullptr)
			++CntBlk_->SharedCount;
	}

	// ������ֵ�����
	TSharedPtr<T>& operator=(const TSharedPtr& Other)
	{
		if (&Other != this)
		{
			// ���ȣ�����ԭ�м���
			CleanUp();

			// �����¼���
			Ptr_ = Other.Ptr_;
			CntBlk_ = Other.CntBlk_;
			if (CntBlk_ != nullptr)
				++CntBlk_->SharedCount;
		}
		return *this;
	}

	// �ƶ�����
	TSharedPtr(TSharedPtr<T>&& Other) noexcept :
		Ptr_(Other.Ptr_),
		CntBlk_(Other.CntBlk_)
	{
		Other.Ptr_ = nullptr;
		Other.CntBlk_ = nullptr;
	}

	// �ƶ���ֵ
	TSharedPtr<T>& operator=(TSharedPtr<T>&& Other) noexcept
	{
		if (&Other != this)
		{
			CleanUp();
			Ptr_ = Other.Ptr_;
			CntBlk_ = Other.CntBlk_;
			Other.Ptr_ = nullptr;
			Other.CntBlk_ = nullptr;
		}
		return *this;
	}

	// ��WeakPtr����
	TSharedPtr(const TWeakPtr<T>& WeakP)
	{
		// �����Դ��Ч
		if (WeakP.CntBlk_ != nullptr && WeakP.CntBlk_.SharedCount > 0)
		{
			Ptr_ = WeakP->Ptr_;
			CntBlk_ = WeakP.CntBlk_;
			++CntBlk_->SharedCount;
		}
	}

	// ����
	~TSharedPtr()
	{
		CleanUp();
	}

public:
	T& operator*()
	{
		return *Ptr_;
	}

	const T& operator*() const
	{
		return *Ptr_;
	}

	T* operator->()
	{
		return Ptr_;
	}

	const T* operator->() const
	{
		return Ptr_;
	}

public:
	size_t GetCount() const
	{
		if (CntBlk_ != nullptr)
			return CntBlk_->SharedCount;
		return 0;
	}

	T* Get()
	{
		return Ptr_;
	}

	const T* Get() const
	{
		return Ptr_;
	}

	bool IsUnique() const
	{
		if (CntBlk_ != nullptr && CntBlk_->SharedCount == 1)
			return true;
		return false;
	}

	bool IsValid() const
	{
		if (CntBlk_ != nullptr && CntBlk_->SharedCount > 0)
			return true;
		return false;
	}

	void Reset(T* NewPtr = nullptr)
	{
		if (Ptr_ != NewPtr)
		{
			CleanUp();
			Ptr_ = NewPtr;
			CntBlk_ = NewPtr == nullptr ? nullptr : new FCountBlock{ 1,0 };
		}
	}

private:
	// ��������
	void CleanUp()
	{
		if (CntBlk_ == nullptr)
			return;

		// ǿ���ü�һ
		--CntBlk_->SharedCount;

		// ��ǿ����Ϊ0
		if (CntBlk_->SharedCount == 0)
		{
			// �ͷ���Դ�ڴ�
			delete Ptr_;

			// ��������Ϊ0, �ͷż������ڴ�
			if (CntBlk_->WeakCount == 0)
				delete CntBlk_;
		}
		Ptr_ = nullptr;
		CntBlk_ = nullptr;
	}

private:
	T* Ptr_;
	FCountBlock* CntBlk_;
};

template<typename T>
class TWeakPtr
{
public:
	friend class TSharedPtr<T>;

public:
	TWeakPtr() :
		Ptr_(nullptr),
		CntBlk_(nullptr)
	{
	}

	// ��SharedPtr����
	TWeakPtr(const TSharedPtr<T>& SharedP) :
		Ptr_(SharedP.Ptr_),
		CntBlk_(SharedP.CntBlk_)
	{
		if (CntBlk_ != nullptr)
			++CntBlk_->WeakCount;
	}

	// ��������
	TWeakPtr(const TWeakPtr<T>& Other) :
		Ptr_(Other.Ptr_),
		CntBlk_(Other.CntBlk_)
	{
		if (CntBlk_ != nullptr)
			++CntBlk_->WeakCount;
	}

	// ������ֵ
	TWeakPtr<T>& operator=(const TWeakPtr<T>& Other)
	{
		if (&Other != this)
		{
			WeakCleanUp();
			Ptr_ = Other.Ptr_;
			CntBlk_ = Other.CntBlk_;
			if (CntBlk_ != nullptr)
				++CntBlk_->WeakCount;
		}
		return *this;
	}

	// ��SharedPtr��ֵ
	TWeakPtr<T>& operator=(const TSharedPtr<T>& SharedP)
	{
		WeakCleanUp();
		Ptr_ = SharedP.Ptr_;
		CntBlk_ = SharedP.CntBlk_;
		if (CntBlk_ != nullptr)
			++CntBlk_->WeakCount;
		return *this;
	}

	// �ƶ�����
	TWeakPtr(TWeakPtr<T>&& Other) noexcept:
		Ptr_(Other.Ptr_),
		CntBlk_(Other.CntBlk_)
	{
		Other.Ptr_ = nullptr;
		Other.CntBlk_ = nullptr;
	}

	// ��������
	~TWeakPtr()
	{
		WeakCleanUp();
	}

public:
	// ��ȡ��Դ
	T* Get()
	{
		return Ptr_;
	}

	const T* Get() const
	{
		return Ptr_;
	}

	T& operator*()
	{
		return *Ptr_;
	}

	const T& operator*() const
	{
		return *Ptr_;
	}

	T* operator->()
	{
		return Ptr_;
	}

	const T* operator->() const
	{
		return Ptr_;
	}

	// ��Ч�Լ��
	bool IsValid() const
	{
		if (CntBlk_ != nullptr && CntBlk_->SharedCount > 0)
			return true;
		return false;
	}

	// ����ΪSharedPtr
	TSharedPtr<T> Lock()
	{
		if (IsValid())
			return TSharedPtr<T>(*this);
		return TSharedPtr<T>();
	}

public:
	// ��ȡ���ü���
	size_t GetSharedCount() const
	{
		if (CntBlk_ != nullptr)
			return CntBlk_->SharedCount;
		return 0;
	}

	// ��ȡ�����ü���
	size_t GetWeakCount() const
	{
		if (CntBlk_ != nullptr)
			return CntBlk_->WeakCount;
		return 0;
	}

private:
	void WeakCleanUp()
	{
		if (CntBlk_ != nullptr)
		{
			--CntBlk_->WeakCount;

			if (CntBlk_->SharedCount == 0 && CntBlk_->WeakCount == 0)
				delete CntBlk_;
		}
		Ptr_ = nullptr;
		CntBlk_ = nullptr;
	}

private:
	T* Ptr_;
	typename TSharedPtr<T>::FCountBlock* CntBlk_;
};
}