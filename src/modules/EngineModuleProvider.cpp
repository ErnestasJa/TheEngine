#include "Precomp.h"
#include "EngineModuleProvider.h"

EngineModuleProvider::EngineModuleProvider()
{

}

EngineModuleProvider::~EngineModuleProvider()
{

}

IEngineModuleWeakPtr EngineModuleProvider::GetModuleWithType(ModuleType type)
{
	auto typeComp = [&type](const IEngineModulePtr & ptr){ return ptr->GetType() == type; };
	auto it = std::find_if(m_engineModules.begin(), m_engineModules.end(), typeComp);
	return it != m_engineModules.end() ? *it : nullptr; 
}

bool EngineModuleProvider::HasModuleWithType(ModuleType type)
{
	auto typeComp = [&type](const IEngineModulePtr & ptr){ return ptr->GetType() == type; };
	auto it = std::find_if(m_engineModules.begin(), m_engineModules.end(), typeComp);
	return it != m_engineModules.end(); 
}

bool EngineModuleProvider::RemoveModuleWithType(ModuleType type)
{
	auto typeComp = [&type](const IEngineModulePtr & ptr){ return ptr->GetType() == type; };
	auto it = std::find_if(m_engineModules.begin(), m_engineModules.end(), typeComp);
	bool found = it != m_engineModules.end();
	m_engineModules.erase(it);

	return found;
}

void EngineModuleProvider::RegisterModule(IEngineModulePtr module, ModuleType type)
{
	if(HasModuleWithType(type))
	{
		RemoveModuleWithType(type);
	}
	else
	{
		m_engineModules.push_back(module);
	}
}
