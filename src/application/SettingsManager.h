#ifndef SETTINGS_MANAGER
#define SETTINGS_MANAGER

#include "AppContext.h"
#include "core/VarJsonReader.h"
#include "core/Var.h"
#include "core/VarGroup.h"
#include "core/VarJsonReader.h"
#include "core/FileSystemHelpers.h"

class ApplicationSettingsManager: public VarGroup
{
public:
	ApplicationSettingsManager(AppContext * appContext) : VarGroup("settings")
	{
		m_appContext = appContext;
		InitSettings();		
	}


	void LoadSettings(const std::string & fileName)
	{
		VarJsonReader reader;
		reader.Load(fileName, *this);
	}

	void WriteSettings(const std::string & fileName)
	{
		VarJsonReader jsonWriter; // whoa whoa, it can write..
		jsonReader.Write(fileName, *this);
	}

private:
	void InitSettings()
	{
		VarGroup & fs = this->AddGroup("filesystem");
		fs.AddVar(Var("resource_path", "res"));
		fs.AddVar(Var("engine_resource_path", "engine"));
		fs.AddVar(Var("config_path", "conf"));
		fs.AddVar(Var("log_path", "log"));	
	}

private:
	AppContext * m_appContext;
};

#endif