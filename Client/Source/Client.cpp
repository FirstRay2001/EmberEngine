// Client.cpp
// ��Ϸ�ͻ������
// created by FirstRay2001, Oct/30/2025

#include <Ember.h>
#include <iostream>

class GameClient : public Ember::Application
{
public:
	GameClient() 
	{
		EMBER_INFO("GameClient initialized!");
	}

	~GameClient() {}
};

EMBER_IMPL_ENTRY_POINT(GameClient);