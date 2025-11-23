// Math.h
// 数学库
// created by FirstRay2001, Nov/23/2025

#pragma once

#include <glm/glm.hpp>

namespace Ember::Math
{
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);
}
