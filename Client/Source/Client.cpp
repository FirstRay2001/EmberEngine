// Client.cpp
// 游戏客户端入口
// created by FirstRay2001, Oct/30/2025

#include <Ember.h>
#include <Ember/Core/EntryPoint.h>
#include "PortalGameLayer.h"

class GameClient : public Ember::Application
{
public:
	GameClient() 
	{
		PushLayer(new PortalGameLayer());
	}

	~GameClient() {}
};

EMBER_IMPL_ENTRY_POINT(GameClient);