// Transform.h
// ÈýÎ¬±ä»»Àà
// created by FirstRay2001, Sep/17/2025

#pragma once

#include "Common.h"
#include "Source/Scene/Rotator.h"

class FTransform
{
public:
	FTransform(MySTL::TSharedPtr<FTransform>& Parent);
	
	void UpdateAbsolute();

private:
	MySTL::TWeakPtr<FTransform>					Parent_;
	MySTL::TVector<MySTL::TWeakPtr<FTransform>>	Children_;
	MyMath::FVector3							AbsoluteLocation_;
	FRotator									AbsoluteRotation_;
	MyMath::FVector3							AbsoluteScale_;
	MyMath::FVector3							RelativeLocation_;
	FRotator									RelativeRotation_;
	MyMath::FVector3							RelativeScale_;
};