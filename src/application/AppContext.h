#ifndef APP_CONTEXT_H
#define APP_CONTEXT_H

#include "ForwardDecl.h"
#include <boost/core/noncopyable.hpp>

class AppContext;
AppContext & GetContext();

class AppContext: private boost::noncopyable
{
protected:
	AppContext();
	virtual ~AppContext();
public:
	friend class Application;
	static AppContext & Instance();
	bool IsInitialized();

	Logger * GetLogger();
	ApplicationWindow * GetWindow();
	OpenGLExtensionLoader * GetOpenGLExtensionLoader();
	GUIEnvironment * GetGUIEnvironment();
	TimerPtr GetTimer();
	InputHandler * GetInputHandler();
	ApplicationSettingsManager * GetApplicationSettingsManager();
	FileSystem * GetFileSystem();

	void SetInputHandler(InputHandler* inputHandler);
	void SetGUIEnvironment(GUIEnvironment* guiEnvironment);
protected:
	ApplicationWindow * p_window;
	OpenGLExtensionLoader * p_openGLExtensionLoader;
	GUIEnvironment * p_guiEnv;
	TimerPtr p_timer;
	Logger * p_logger; 
	InputHandler * p_inputHandler;
	ApplicationSettingsManager * p_settingsManager;
	FileSystem * p_fileSystem;
};

#endif
