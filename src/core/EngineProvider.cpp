#include "Precomp.h"
#include "EngineProvider.h"

EngineProvider::EngineProvider()
{

}

EngineProvider::~EngineProvider()
{

}

IEngineModuleWeakPtr EngineProvider::GetModuleByType(ModuleType type)
{
	auto typeComp = [&type](const IEngineModulePtr & ptr){ return ptr->GetType() == type; };
	auto it = std::find_if(m_engineModules.begin(), m_engineModules.end(), typeComp);
	return it != m_engineModules.end() ? *it : nullptr; 
}

void EngineProvider::CreateModules(const vector<ModuleType> & modules)
{
	for(auto moduleType : modules)
	{

		if(GetModuleByType(moduleType).expired() == false)
		{
			///NOTE: throw exception when core engine exceptions implemented.
			continue;
		}

		auto func = GetModuleCreateFuncByType(moduleType);
		
		if(func)
		{
			auto module = func();
			m_engineModules.push_back(module);
		}
		else
		{
			///NOTE: log somewhere or make a list of unregistered modules.
		}
	}
}

void EngineProvider::RegisterModule(EngineModuleCreateFunc engineModuleCreateFunc, ModuleType type)
{
	if(!GetModuleCreateFuncByType(type))
	{
		m_engineModuleRegistry[type] = engineModuleCreateFunc;
	}
	else
	{
		///NOTE: exception?
	}
}

EngineModuleCreateFunc EngineProvider::GetModuleCreateFuncByType(ModuleType type)
{
	auto it = m_engineModuleRegistry.find(type);
	return it != m_engineModuleRegistry.end() ? it->second : EngineModuleCreateFunc();
}
	