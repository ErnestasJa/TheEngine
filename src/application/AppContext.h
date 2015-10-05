#pragma once

#include "Window.h"
#include "opengl/OpenGLUtil.h"
#include "gui/GUIEnvironment.h"
#include "utility/Logger.h"
#include "utility/Timer.h"
#include "InputHandler.h"
#include "core/SettingsManager.h"

struct AppContext
{
	AppContext()
	{
		_window = nullptr;
		_guiEnv = nullptr;
		_timer = nullptr;
		_logger = nullptr;
		_glUtil = nullptr;
		_input = nullptr;
		settingsManager = nullptr;
	}

	bool IsInitialized()
	{
		return _window && _glUtil && _timer && _logger && settingsManager;
	}


	//pointless underscores, refactor places where they are used later.
	union 
	{
		Window* window;
		Window* _window;
	};


	union 
	{
		OpenGLUtil * glUtil;
		OpenGLUtil * _glUtil;
	};


	union 
	{
		GUIEnvironment* guiEnv;
		GUIEnvironment* _guiEnv;
	};


	union 
	{
		timer_ptr   timer;
		timer_ptr   _timer;
	};

	union 
	{
		Logger* logger; 
		Logger* _logger;
	};


	union 
	{
		InputHandler* input;
		InputHandler* _input;
	};

	SettingsManager * settingsManager;
};
