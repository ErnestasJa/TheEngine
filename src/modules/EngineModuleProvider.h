#ifndef ENGINE_MODULE_PROVIDER_H
#define ENGINE_MODULE_PROVIDER_H

#include "core/Vector.h"
#include "IEngineModule.h"
#include "ForwardDecl.h"

class EngineModuleProvider
{
public:
	EngineModuleProvider();
	virtual ~EngineModuleProvider();

	IEngineModuleWeakPtr GetModuleWithType(ModuleType type);
	template <class T> std::weak_ptr<T> GetModuleWithType(ModuleType type);
	bool HasModuleWithType(ModuleType type);
	bool RemoveModuleWithType(ModuleType type);
	void RegisterModule(IEngineModulePtr module, ModuleType type);

private:
	vector<IEngineModulePtr> m_engineModules;
};

template <class T> std::weak_ptr<T> EngineModuleProvider::GetModuleWithType(ModuleType type)
{
	return std::static_pointer_cast<T>(GetModuleWithType(type).lock());
}

#endif