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
	ApplicationSettingsManager();
	virtual ~ApplicationSettingsManager();
	bool LoadSettings(const Path & fileName);
	bool WriteSettings(const Path & fileName);

private:
	void InitSettings();
};

#endif