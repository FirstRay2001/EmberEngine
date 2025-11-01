// Ember.h
// 客户端用引擎头
// created by FirstRay2001, Oct/30/2025

#pragma once

#include "Ember/Application.h"
#include "Ember/Log.h"

// -------------入口点------------------------------------
#include "Ember/EntryPoint.h"
#define EMBER_IMPL_ENTRY_POINT(ApplicationClass)		\
	Ember::Application* Ember::CreateApplication()		\
	{													\
		return new ApplicationClass();					\
	}													\
// -------------------------------------------------------

