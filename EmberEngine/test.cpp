#include <iostream>

#include "test.h"
#include "Libraries/Foundation/Include/Collection.h"
#include "Libraries/Foundation/src/Math/MathCommon.h"

#include "Source/Graphics/Mesh.h"
#include "Source/Graphics/Shader.h"
#include "Common.h"
#include "Source/Graphics/Texture.h"

void MyTest()
{
	MySTL::THashTable<int, int> mp;
	mp.Insert(1, 2);
	auto* a = mp.Find(1);
	std::cout << *a;
}
	
