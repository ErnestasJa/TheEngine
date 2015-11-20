#ifndef IENGINE_MODULE_H
#define IENGINE_MODULE_H

#include "ForwardDecl.h"

enum class ModuleType:uint32_t
{
	UNDEFINED = 0,
	FILESYSTEM = 1,
	LOGGING = 2,
	WINDOW = 3,

	COUNT,
	// For modules registered outside the engine scope.
	// Make sure to provide unique ids
	EXTERNAL = 128 
};

class IEngineModule
{
public:
	virtual ~IEngineModule(){}
	virtual bool ResolveDependantModules(std::weak_ptr<EngineProvider> ptr);
	virtual bool IsInitialized() const = 0;
	virtual const std::string & GetName() const = 0;
	virtual ModuleType GetType() const = 0;
	virtual uint32_t GetExternalType() const = 0;
};

typedef std::function<IEngineModulePtr(void)> EngineModuleCreateFunc;

#endif