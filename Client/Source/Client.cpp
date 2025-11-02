// Client.cpp
// 游戏客户端入口
// created by FirstRay2001, Oct/30/2025

#include <Ember.h>
#include <iostream>

class TestLayer : public Ember::Layer
{
public:
	TestLayer() : Ember::Layer("TestLayer") {}

	void OnEvent(Ember::Event& e) override
	{
		EMBER_INFO("Event received in TestLayer: {0}", e.ToString());
	}
};

class GameClient : public Ember::Application
{
public:
	GameClient() 
	{
		PushLayer(new TestLayer());
	}

	~GameClient() {}
};

EMBER_IMPL_ENTRY_POINT(GameClient);