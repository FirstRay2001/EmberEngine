// HashTable.h
// 哈希表
// created by FirstRay2001, Sep/19/2025

#pragma once

#include<utility>	// TODO: 实现自己的Hash算法，而不是使用标准库
#include <stdexcept>
#include "Vector.h"

namespace MySTL
{
template<typename TK, typename TV, typename THash = std::hash<TK>>
class THashTable 
{
private:
	// 键值对结点
	struct Node
	{
		TK Key_;
		TV Value_;
		Node* Next_;

		template<typename K, typename V>
		Node(K&& k, V&& v, Node* n = nullptr) :
			Key_(k),
			Value_(v),
			Next_(n)
		{
		}
	};

public:
	THashTable(size_t BucketCount = 16, const THash& Hash = THash()) :
		Buckets_(BucketCount, nullptr),
		Size_(0),
		Hash_(Hash),
		MaxLoadFactor(1.0f)
	{ 
	}

	~THashTable()
	{
		Clear();
	}

public:
	template<typename K, typename V>
	bool Insert(K&& Key, V&& Value)
	{
		// 检查是否需要扩容
		if (LoadFactor() > MaxLoadFactor)
			Rehash(Buckets_.Size() * 2);

		size_t Index = BucketIndex(Key);
		Node* Current = Buckets_[Index];

		// 检查Key是否已存在
		while (Current != nullptr)
		{
			if (Current->Key_ == Key)
			{
				// 更新值
				Current->Value_ = Value;
				return false;
			}
			Current = Current->Next_;
		}

		// 插入新节点到链表头部
		Node* NewNode = new Node(std::forward<K>(Key), std::forward<V>(Value));
		NewNode->Next_ = Buckets_[Index];
		Buckets_[Index] = NewNode;
		++Size_;

		return true;
	}

	bool Erase(const TK& Key)
	{
		size_t Index = BucketIndex(Key);
		Node* Current = Buckets_[Index];
		Node* Prev = nullptr;

		while (Current != nullptr)
		{
			if (Current->Key_ == Key)
			{
				if (Prev != nullptr)
					Prev->Next_ = Current->Next_;
				else
					Buckets_[Index] = Current->Next_;

				delete Current;
				--Size_;
				return true;
			}

			Prev = Current;
			Current = Current->Next_;
		}

		return false;
	}

	void Clear()
	{
		for (size_t i = 0; i < Buckets_.Size(); ++i)
		{
			Node* Current = Buckets_[i];
			while (Current != nullptr)
			{
				Node* Next = Current->Next_;
				delete Current;
				Current = Next;
			}
			Buckets_[i] = nullptr;
		}
		Size_ = 0;
	}

	TV& operator[](const TK& Key)
	{
		TV* Res = Find(Key);

		if (Res != nullptr)
			return *Res;

		// 未找到，插入新键
		return InsertAndReturn(Key, TV{});
	}

	const TV& operator[](const TK& Key) const
	{
		TV* Res = Find(Key);

		if (Res != nullptr)
			return *Res;

		throw std::out_of_range("Key not found in HashTable");
	}

	TV* Find(const TK& Key) const
	{
		size_t Index = BucketIndex(Key);
		Node* Current = Buckets_[Index];

		while (Current != nullptr)
		{
			if (Current->Key_ == Key)
				return &Current->Value_;
			Current = Current->Next_;
		}

		return nullptr;
	}

private:
	// 获取桶索引
	size_t BucketIndex(const TK& Key) const
	{
		return Hash_(Key) % Buckets_.Size();
	}

	// 扩容
	void Rehash(size_t NewBucketSize)
	{
		if (NewBucketSize < Buckets_.Size())
			return;

		// 分配新内存
		TVector<Node*> NewBucket(NewBucketSize, nullptr);

		// 重新计算现有元素的位置
		for (size_t i = 0; i < Buckets_.Size(); ++i)
		{
			Node* Current = Buckets_[i];
			while (Current)
			{
				Node* Next = Current->Next_;
				size_t NewIndex = Hash_(Current->Key_) % NewBucketSize;
				Current->Next_ = NewBucket[NewIndex];
				NewBucket[NewIndex] = Current;
				Current = Next;
			}
		}

		// 移动赋值
		Buckets_ = std::move(NewBucket);
	}

	// 计算负载因子
	float LoadFactor() const
	{
		return Buckets_.Empty() ? 0.0f : static_cast<float>(Size_) / Buckets_.Size();
	}

	TV& InsertAndReturn(const TK& Key, TV&& Value) 
	{
		// 检查扩容
		if (LoadFactor() > MaxLoadFactor) {
			Rehash(Buckets_.Size() * 2);
		}

		size_t Index = BucketIndex(Key);
		Node* NewNode = new Node(Key, std::move(Value));
		NewNode->Next_ = Buckets_[Index];
		Buckets_[Index] = NewNode;
		++Size_;

		return NewNode->Value_;
	}

private:
	TVector<Node*>	Buckets_;
	size_t			Size_;
	THash			Hash_;
	float			MaxLoadFactor;
};
}