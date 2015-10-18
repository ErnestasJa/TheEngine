#include "Precomp.h"
#include "SettingsManager.h"


ApplicationSettingsManager::ApplicationSettingsManager() : VarGroup("settings")
{
	InitSettings();		
}

ApplicationSettingsManager::~ApplicationSettingsManager()
{

}

bool ApplicationSettingsManager::LoadSettings(const Path & fileName)
{
	VarJsonReader reader;
	return reader.Read(fileName, *this);
}

bool ApplicationSettingsManager::WriteSettings(const Path & fileName)
{
	VarJsonReader jsonWriter; // whoa whoa, it can write..
	return jsonWriter.Write(fileName, *this);
}

void ApplicationSettingsManager::InitSettings()
{
	VarGroup & fileSystemVarGroup = this->AddGroup("filesystem");
	fileSystemVarGroup.AddVar(Var("config_path", "config"));
	fileSystemVarGroup.AddVar(Var("log_path", "log"));	

	VarGroup & videoVarGroup = this->AddGroup("video");
	videoVarGroup.AddVar(Var("window_width", 1024));
	videoVarGroup.AddVar(Var("window_height", 768));
}