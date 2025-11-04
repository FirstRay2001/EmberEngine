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
		if (e.GetEventType() == Ember::EventType::KeyPressed)
		{
			Ember::KeyPressedEvent& event = (Ember::KeyPressedEvent&)e;
			EMBER_TRACE("Key typed: {0}", (char)event.GetKeyCode());
		}
	}
};

class GameClient : public Ember::Application
{
public:
	GameClient() 
	{
		PushLayer(new TestLayer());
		PushLayer(new Ember::ImGuiLayer());
	}

	~GameClient() {}
};

EMBER_IMPL_ENTRY_POINT(GameClient);