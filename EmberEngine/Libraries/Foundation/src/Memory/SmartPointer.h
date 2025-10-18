// SmartPointer.h
// 智能指针
// created by FirstRay2001, Sep/15/2025

#pragma once

namespace MySTL
{
template<typename T>
class TUniquePtr
{
public:
	// 禁用拷贝构造
	TUniquePtr(TUniquePtr<T>& Other) = delete;

	// 接管原始指针
	TUniquePtr(T* OriginalPtr = nullptr) :
		Ptr_(OriginalPtr)
	{
	}

	// 移动构造
	TUniquePtr(TUniquePtr<T>&& Other) noexcept :
		Ptr_(Other.Ptr_)
	{
		// 释放另一个指针资源
		Other.Ptr_ = nullptr;
	}

	// 移动赋值
	TUniquePtr<T>& operator=(TUniquePtr<T>&& Other) noexcept
	{
		// 非自我赋值
		if (&Other != this)
		{
			// 释放旧资源
			delete Ptr_;

			// 接管新资源
			Ptr_ = Other.Ptr_;
			Other.Ptr_ = nullptr;
		}
		return *this;
	}

	// 析构
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

	// 释放所有权，返回原始指针
	T* Release()
	{
		T* OldPtr = Ptr_;
		Ptr_ = nullptr;
		return OldPtr;
	}

	// 重置指针
	void Reset(T* NewPtr = nullptr) noexcept
	{
		if (NewPtr == Ptr_) return;
		T* OldPtr = Ptr_;
		Ptr_ = NewPtr;
		delete OldPtr;
	}

	// 有效性检查
	explicit operator bool() const
	{
		return Ptr_ != nullptr;
	}

private:
	T* Ptr_;
};

// 友元前置声明
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
	// 初始化计数
	explicit TSharedPtr(T* OriginalPtr = nullptr) :
		Ptr_(OriginalPtr),
		CntBlk_(OriginalPtr == nullptr ? nullptr : new FCountBlock{ 1, 0 })
	{
	}

	// 拷贝构造
	TSharedPtr(const TSharedPtr& Other) :
		Ptr_(Other.Ptr_),
		CntBlk_(Other.CntBlk_)
	{
		if (CntBlk_ != nullptr)
			++CntBlk_->SharedCount;
	}

	// 拷贝赋值运算符
	TSharedPtr<T>& operator=(const TSharedPtr& Other)
	{
		if (&Other != this)
		{
			// 首先，减少原有计数
			CleanUp();

			// 增加新计数
			Ptr_ = Other.Ptr_;
			CntBlk_ = Other.CntBlk_;
			if (CntBlk_ != nullptr)
				++CntBlk_->SharedCount;
		}
		return *this;
	}

	// 移动构造
	TSharedPtr(TSharedPtr<T>&& Other) noexcept :
		Ptr_(Other.Ptr_),
		CntBlk_(Other.CntBlk_)
	{
		Other.Ptr_ = nullptr;
		Other.CntBlk_ = nullptr;
	}

	// 移动赋值
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

	// 从WeakPtr构造
	TSharedPtr(const TWeakPtr<T>& WeakP)
	{
		// 如果资源有效
		if (WeakP.CntBlk_ != nullptr && WeakP.CntBlk_.SharedCount > 0)
		{
			Ptr_ = WeakP->Ptr_;
			CntBlk_ = WeakP.CntBlk_;
			++CntBlk_->SharedCount;
		}
	}

	// 析构
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
	// 减少引用
	void CleanUp()
	{
		if (CntBlk_ == nullptr)
			return;

		// 强引用减一
		--CntBlk_->SharedCount;

		// 若强引用为0
		if (CntBlk_->SharedCount == 0)
		{
			// 释放资源内存
			delete Ptr_;

			// 若弱引用为0, 释放计数块内存
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

	// 从SharedPtr构造
	TWeakPtr(const TSharedPtr<T>& SharedP) :
		Ptr_(SharedP.Ptr_),
		CntBlk_(SharedP.CntBlk_)
	{
		if (CntBlk_ != nullptr)
			++CntBlk_->WeakCount;
	}

	// 拷贝构造
	TWeakPtr(const TWeakPtr<T>& Other) :
		Ptr_(Other.Ptr_),
		CntBlk_(Other.CntBlk_)
	{
		if (CntBlk_ != nullptr)
			++CntBlk_->WeakCount;
	}

	// 拷贝赋值
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

	// 从SharedPtr赋值
	TWeakPtr<T>& operator=(const TSharedPtr<T>& SharedP)
	{
		WeakCleanUp();
		Ptr_ = SharedP.Ptr_;
		CntBlk_ = SharedP.CntBlk_;
		if (CntBlk_ != nullptr)
			++CntBlk_->WeakCount;
		return *this;
	}

	// 移动构造
	TWeakPtr(TWeakPtr<T>&& Other) noexcept:
		Ptr_(Other.Ptr_),
		CntBlk_(Other.CntBlk_)
	{
		Other.Ptr_ = nullptr;
		Other.CntBlk_ = nullptr;
	}

	// 析构函数
	~TWeakPtr()
	{
		WeakCleanUp();
	}

public:
	// 获取资源
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

	// 有效性检查
	bool IsValid() const
	{
		if (CntBlk_ != nullptr && CntBlk_->SharedCount > 0)
			return true;
		return false;
	}

	// 提升为SharedPtr
	TSharedPtr<T> Lock()
	{
		if (IsValid())
			return TSharedPtr<T>(*this);
		return TSharedPtr<T>();
	}

public:
	// 获取引用计数
	size_t GetSharedCount() const
	{
		if (CntBlk_ != nullptr)
			return CntBlk_->SharedCount;
		return 0;
	}

	// 获取弱引用计数
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