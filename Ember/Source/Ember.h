// Ember.h
// �ͻ���������ͷ
// created by FirstRay2001, Oct/30/2025

#pragma once

#include "Ember/Application.h"
#include "Ember/Log.h"

// -------------��ڵ�------------------------------------
#include "Ember/EntryPoint.h"
#define EMBER_IMPL_ENTRY_POINT(ApplicationClass)		\
	Ember::Application* Ember::CreateApplication()		\
	{													\
		return new ApplicationClass();					\
	}													\
// -------------------------------------------------------

