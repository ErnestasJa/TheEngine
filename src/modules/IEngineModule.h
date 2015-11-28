#ifndef IENGINE_MODULE_H
#define IENGINE_MODULE_H

#include "ForwardDecl.h"

enum class ModuleType:uint32_t
{
	Undefined = 0,
	FileSystem = 1,
	Logging = 2,
	Window = 3,

	COUNT,
	// For modules registered outside the engine scope.
	// Make sure to provide unique ids
	EXTENDED = 128 
};

// Module is a part of application that has only one instance per application lifetime.
class IEngineModule
{
public:
	virtual ~IEngineModule(){}
	virtual bool Initialize(EngineModuleProviderWeakPtr ptr) = 0;
	virtual bool IsInitialized() const = 0;
	virtual const std::string & GetName() const = 0;
	virtual const ModuleType GetType() const = 0;
	virtual uint32_t GetExtendedType() const = 0;
};

typedef std::function<IEngineModulePtr(void)> EngineModuleCreateFunc;

#endif