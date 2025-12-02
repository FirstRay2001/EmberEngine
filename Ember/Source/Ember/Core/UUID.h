// UUID.h
// 全局唯一ID
// created by FirstRay2001, Nov/28/2025

#pragma once

namespace Ember
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID& other) = default;

		operator uint64_t() const { return m_UUID; }

	private:
		uint64_t m_UUID;
	};
}

namespace std 
{
	template <typename T> struct hash;

	// 显示专有化
	template<>
	struct hash<Ember::UUID>
	{
		std::size_t operator()(const Ember::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};

}