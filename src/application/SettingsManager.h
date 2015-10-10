#ifndef SETTINGS_MANAGER
#define SETTINGS_MANAGER

#include "core/VarJsonReader.h"
#include "core/Var.h"
#include "core/VarGroup.h"
#include "core/VarJsonReader.h"

class AppContext;
class ApplicationSettingsManager: public VarGroup
{
public:
	ApplicationSettingsManager(AppContext * appContext);
	bool LoadSettings(const Path & fileName);
	bool WriteSettings(const Path & fileName);

private:
	void InitSettings();

private:
	AppContext * m_appContext;
};

#endif