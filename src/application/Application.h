#pragma once

#include "ForwardDecl.h"
#include "AppContext.h"
///REFACTOR: remove these includes
#include "core/VarJsonReader.h"
#include "core/Var.h"
#include "core/VarGroup.h"

class ApplicationWindow;

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
	[[deprecated]] VarGroup & GetEngineVars();

protected:
	int32_t m_argc;
	const char ** m_argv;

	bool InitWindowAndOpenGL(const std::string & title);
	void InitFileSystemAndLoadConfig();
};