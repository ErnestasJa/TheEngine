#include "Precomp.h"
#include "AppContext.h"
#include "SettingsManager.h"


ApplicationSettingsManager::ApplicationSettingsManager(AppContext * appContext) : VarGroup("settings")
{
	m_appContext = appContext;
	InitSettings();		
}


void ApplicationSettingsManager::LoadSettings(const Path & fileName)
{
	VarJsonReader reader(m_appContext->logger);
	reader.Read(fileName, *this);
}

void ApplicationSettingsManager::WriteSettings(const Path & fileName)
{
	VarJsonReader jsonWriter(m_appContext->logger); // whoa whoa, it can write..
	jsonReader.Write(fileName, *this);
}

void ApplicationSettingsManager::InitSettings()
{
	VarGroup & fileSystemVarGroup = this->AddGroup("filesystem");
	fileSystemVarGroup.AddVar(Var("config_path", "config"));
	fileSystemVarGroup.AddVar(Var("log_path", "log"));	

	VarGroup & videoVarGroup = this->AddGroup("video");
	videoVarGroup.AddVar(Var("window_width", "1024"));
	videoVarGroup.AddVar(Var("window_height", "768"));
}