// Texture.h
// 纹理类
// created by FirstRay2001, Nov/6/2025

#pragma once

#include "Ember/Core/Core.h"

namespace Ember
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
	};
}
