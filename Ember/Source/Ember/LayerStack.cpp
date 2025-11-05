// LayerStack.h
// 图层堆栈
// created by FirstRay2001, Nov/2/2025

#include "emberpch.h"
#include "LayerStack.h"

namespace Ember
{

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		layer->OnAttach();
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto It = std::find(m_Layers.begin(), m_Layers.end(), layer);

		// 暂时弹出图层，不进行销毁
		if (It != m_Layers.end())
		{
			(*It)->OnDetach();
			m_Layers.erase(It);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto It = std::find(m_Layers.begin(), m_Layers.end(), overlay);

		// 暂时弹出图层，不进行销毁
		if (It != m_Layers.end())
		{
			(*It)->OnDetach();
			m_Layers.erase(It);
		}
	}
}