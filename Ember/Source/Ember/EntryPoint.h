// EntryPoint.h
// ��ڵ�
// created by FirstRay2001, Oct/30/2025

#pragma once


#ifdef EMBER_PLATFORM_WINDOWS

// �ڿͻ���ʵ�ִ˺����Դ���Ӧ��ʵ��
extern Ember::Application* Ember::CreateApplication();

int main(int argc, char** argv)
{
	// ��ʼ����־ϵͳ
	Ember::Log::Init();

	EMBER_CORE_INFO("Ember Engine started!");

	// ����Ӧ��ʵ��
	Ember::Application* app = Ember::CreateApplication();

	// ����Ӧ��
	app->Run();

	// �ͷ�
	delete app;

	return 0;
}

#else
	#error Ember only supports Windows!
#endif