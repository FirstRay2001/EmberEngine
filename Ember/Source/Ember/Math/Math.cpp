// Math.h
// 数学库
// created by FirstRay2001, Nov/23/2025

#include "emberpch.h"
#include "Math.h"

namespace Ember::Math
{
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale)
	{
		//////// 提取平移分量 ////////
		outTranslation = glm::vec3(transform[3]);

		//////// 提取缩放分量 ////////
		outScale.x = glm::length(glm::vec3(transform[0]));
		outScale.y = glm::length(glm::vec3(transform[1]));
		outScale.z = glm::length(glm::vec3(transform[2]));

		//////// 提取旋转分量 ////////
		glm::mat4 rotationMatrix = transform;

		// 去除缩放 
		if (outScale.x != 0) rotationMatrix[0] /= outScale.x;
		if (outScale.y != 0) rotationMatrix[1] /= outScale.y;
		if (outScale.z != 0) rotationMatrix[2] /= outScale.z;

		// 计算欧拉角（弧度）
		outRotation.y = asin(-rotationMatrix[0][2]);
		if (cos(outRotation.y) != 0) {
			outRotation.x = atan2(rotationMatrix[1][2], rotationMatrix[2][2]);
			outRotation.z = atan2(rotationMatrix[0][1], rotationMatrix[0][0]);
		}
		else {
			outRotation.x = atan2(-rotationMatrix[2][0], rotationMatrix[1][1]);
			outRotation.z = 0;
		}

		// 转换为度
		outRotation = glm::degrees(outRotation);

		return true;
	}
}