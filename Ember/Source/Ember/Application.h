// Application.h
// Ӧ����
// created by FirstRay2001, Oct/30/2025

#pragma once

#include "Core.h"

namespace Ember
{
	class EMBER_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// �ڿͻ���ʵ�ִ˺����Դ���Ӧ��ʵ��
	Application* CreateApplication();
}