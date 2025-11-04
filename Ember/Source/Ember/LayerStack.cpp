// LayerStack.h
// 图层堆栈
// created by FirstRay2001, Nov/2/2025

#include "emberpch.h"
#include "LayerStack.h"

namespace Ember
{

	LayerStack::LayerStack()
	{
		m_LayerInsert = m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
		m_LayerInsert++; // 存疑
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto It = std::find(m_Layers.begin(), m_Layers.end(), layer);

		// 暂时弹出图层，不进行销毁
		if (It != m_Layers.end())
		{
			m_Layers.erase(It);
			m_LayerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto It = std::find(m_Layers.begin(), m_Layers.end(), overlay);

		// 暂时弹出图层，不进行销毁
		if (It != m_Layers.end())
		{
			m_Layers.erase(It);
		}
	}
}