// Client.cpp
// 游戏客户端入口
// created by FirstRay2001, Oct/30/2025

#include <Ember.h>
#include <iostream>

#include "imgui/imgui.h"

class TestLayer : public Ember::Layer
{
public:
	TestLayer() : Ember::Layer("TestLayer") {}

	virtual void OnEvent(Ember::Event& e) override
	{
		if (e.GetEventType() == Ember::EventType::KeyPressed)
		{
			Ember::KeyPressedEvent& event = (Ember::KeyPressedEvent&)e;
			EMBER_TRACE("Key typed: {0}", (char)event.GetKeyCode());
		}
	}

	virtual void OnImGuiRender() override
	{
		//ImGui::Begin("TestLayer");
		//ImGui::Text("Hello from TestLayer!");
		//ImGui::End();
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