// Client.cpp
// 游戏客户端入口
// created by FirstRay2001, Oct/30/2025

#include <Ember.h>
#include <Ember/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace Ember
{
	class EditorApp : public Application
	{
	public:
		EditorApp()
		{
			PushLayer(new EditorLayer());
		}

		~EditorApp() {}
	};

	EMBER_IMPL_ENTRY_POINT(EditorApp);
}

