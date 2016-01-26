#include "Precomp.h"
#include "AppContext.h"

AppContext & GetContext()
{
	return AppContext::Instance();
}

AppContext::AppContext()
{
	p_window = nullptr;
	p_guiEnv = nullptr;
	p_timer = nullptr;
	p_logger = nullptr;
	p_openGLExtensionLoader = nullptr;
	p_inputHandler = nullptr;
	p_settingsManager = nullptr;
	p_fileSystem = nullptr;
	p_resourceManager = nullptr;
}

AppContext::~AppContext()
{

}


///c++11 way for singletons
AppContext & AppContext::Instance()
{
    static AppContext instance;
    return instance;
}

bool AppContext::IsInitializedNoGL()
{
	return p_timer && p_logger && p_settingsManager && p_fileSystem && p_resourceManager;
}

bool AppContext::IsInitialized()
{
	return p_window && p_timer && p_logger && p_settingsManager && p_fileSystem && p_openGLExtensionLoader && p_resourceManager;
}


FileSystem * AppContext::GetFileSystem()
{
	return p_fileSystem;
}

Logger * AppContext::GetLogger()
{
	return p_logger;
}

ApplicationWindow * AppContext::GetWindow()
{
	return p_window;
}

OpenGLExtensionLoader * AppContext::GetOpenGLExtensionLoader()
{
	return p_openGLExtensionLoader;
}

ResourceManager * AppContext::GetResourceManager()
{
	return p_resourceManager;
}

GUIEnvironment * AppContext::GetGUIEnvironment()
{
	return p_guiEnv;
}

TimerPtr AppContext::GetTimer()
{
	return p_timer;
}

InputHandler * AppContext::GetInputHandler()
{
	return p_inputHandler;
}

ApplicationSettingsManager * AppContext::GetApplicationSettingsManager()
{
	return p_settingsManager;
}

void AppContext::SetInputHandler(InputHandler* inputHandler)
{
	p_inputHandler = inputHandler;
}

void AppContext::SetGUIEnvironment(GUIEnvironment* guiEnvironment)
{
	p_guiEnv = guiEnvironment;
}
