// HashTable.h
// ��ϣ��
// created by FirstRay2001, Sep/19/2025

#pragma once

#include<utility>	// TODO: ʵ���Լ���Hash�㷨��������ʹ�ñ�׼��
#include <stdexcept>
#include "Vector.h"

namespace MySTL
{
template<typename TK, typename TV, typename THash = std::hash<TK>>
class THashTable 
{
private:
	// ��ֵ�Խ��
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
		// ����Ƿ���Ҫ����
		if (LoadFactor() > MaxLoadFactor)
			Rehash(Buckets_.Size() * 2);

		size_t Index = BucketIndex(Key);
		Node* Current = Buckets_[Index];

		// ���Key�Ƿ��Ѵ���
		while (Current != nullptr)
		{
			if (Current->Key_ == Key)
			{
				// ����ֵ
				Current->Value_ = Value;
				return false;
			}
			Current = Current->Next_;
		}

		// �����½ڵ㵽����ͷ��
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

		// δ�ҵ��������¼�
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
	// ��ȡͰ����
	size_t BucketIndex(const TK& Key) const
	{
		return Hash_(Key) % Buckets_.Size();
	}

	// ����
	void Rehash(size_t NewBucketSize)
	{
		if (NewBucketSize < Buckets_.Size())
			return;

		// �������ڴ�
		TVector<Node*> NewBucket(NewBucketSize, nullptr);

		// ���¼�������Ԫ�ص�λ��
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

		// �ƶ���ֵ
		Buckets_ = std::move(NewBucket);
	}

	// ���㸺������
	float LoadFactor() const
	{
		return Buckets_.Empty() ? 0.0f : static_cast<float>(Size_) / Buckets_.Size();
	}

	TV& InsertAndReturn(const TK& Key, TV&& Value) 
	{
		// �������
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