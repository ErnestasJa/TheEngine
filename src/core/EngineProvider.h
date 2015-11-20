#ifndef ENGINE_PROVIDER_H
#define ENGINE_PROVIDER_H

#include "core/Vector.h"
#include "IEngineModule.h"
#include "ForwardDecl.h"

class EngineProvider
{
public:
	EngineProvider();
	virtual ~EngineProvider();

	IEngineModuleWeakPtr GetModuleByType(ModuleType type);
	void CreateModules(const vector<ModuleType> & modules);
	void RegisterModule(EngineModuleCreateFunc engineModuleCreateFunc, ModuleType type);
	
private:
	EngineModuleCreateFunc GetModuleCreateFuncByType(ModuleType type);

private:
	vector<IEngineModulePtr> m_engineModules;
	std::map<ModuleType, EngineModuleCreateFunc> m_engineModuleRegistry;
};


#endif