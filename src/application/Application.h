#pragma once

class Window;
class OpenGLUtil;
class Logger;

#include "AppContext.h"

#include "core/VarJsonReader.h"
#include "core/Var.h"
#include "core/VarGroup.h"

class Application
{
public:

	Application(int32_t argc, const char ** argv);
	virtual ~Application();

	virtual bool Init(const std::string  &title);
	virtual bool Update() = 0;
	virtual void Exit();

	virtual void OnWindowClose() = 0;

	virtual std::string GetApplicationId() = 0;
	virtual AppContext * Ctx();

	[[deprecated]]
	VarGroup & GetEngineVars();

protected:
	AppContext * m_appContext;
	int32_t m_argc;
	const char ** m_argv;

	void InitVariables();
	void InitSettings();
	bool InitWindowAndOpenGL(const std::string & title);
	void InitFileSystemAndLoadConfig();
};
