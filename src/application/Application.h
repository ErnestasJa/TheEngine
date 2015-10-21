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
	virtual bool Init() = 0;
	virtual bool Update() = 0;
	virtual void Exit() = 0;
	virtual void OnWindowClose() = 0;
	virtual std::string GetApplicationId() = 0;
	[[deprecated]] VarGroup & GetEngineVars();

protected:
	bool InitSimple(const std::string & title);
	bool InitFileSystem();
	bool InitWindowAndOpenGL(const std::string & title);
	bool LoadConfig();

private:
	bool DestroyContext();

protected:
	///make argv, argc private and add get/set for them.
	int32_t m_argc;
	const char ** m_argv;
};