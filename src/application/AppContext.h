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
	OpenGLUtil * GetOpenGLUtil();
	GUIEnvironment * GetGUIEnvironment();
	TimerPtr GetTimer();
	InputHandler * GetInputHandler();
	ApplicationSettingsManager * GetApplicationSettingsManager();
	FileSystem * GetFileSystem();

protected:
	ApplicationWindow * p_window;
	OpenGLUtil * p_openGLUtil;
	GUIEnvironment * p_guiEnv;
	TimerPtr p_timer;
	Logger * p_logger; 
	InputHandler * p_inputHandler;
	ApplicationSettingsManager * p_settingsManager;
	FileSystem * p_fileSystem;
};

#endif
