// Transform.cpp
// ÈýÎ¬±ä»»Àà
// created by FirstRay2001, Sep/17/2025

#include "Transform.h"

FTransform::FTransform(MySTL::TSharedPtr<FTransform>& Parent) :
	Parent_(Parent),
	Children_(),
	AbsoluteLocation_(0,0,0),
	AbsoluteRotation_(),
	AbsoluteScale_(1,1,1),
	RelativeLocation_(0,0,0),
	RelativeRotation_(),
	RelativeScale_(1,1,1)
{
}

void FTransform::UpdateAbsolute()
{
}

