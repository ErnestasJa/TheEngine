#ifndef LOGGER_H
#define LOGGER_H

#include "ForwardDecl.h"
#include "modules/IEngineModule.h"

//logger levels
///REFACTOR: bitwise levels, verbosity from config, separate verbosity for file, console.
enum loglevel
{
	LOG_LOG = 0, //just logging
	LOG_DEBUG, //output for debugging
	LOG_WARN, //warnings               ///Why the **** is this the only enum that has the word cut off half way..
	LOG_ERROR, //errors
	LOG_CRITICAL //we're fucked
};

class AppContext;
class Logger: public IEngineModule
{
public:
	Logger(int verbosity); //to be decided
	//verbosity 0: all output
	//verbosity 1: only warnings and errors
	//verbosity 2: only errors
	~Logger();

	virtual bool Initialize(EngineModuleProviderWeakPtr ptr);
	virtual bool IsInitialized() const;
	virtual const std::string & GetName() const;
	virtual const ModuleType GetType() const;
	virtual uint32_t GetExtendedType() const;

	void log(loglevel lev, const char* st, ...);
	void SetLogFile(IFilePtr file);

private:
	Path GenerateLogFileName();

private:
	int m_verbosity;
	IFilePtr m_logfile;
	//vector< std::pair<debuglevel,std::string> > outputs; //wrapped outputs for easy output

	std::string GenerateTimestamp(); //timestamp generation
	std::string FormatMessage(loglevel lev, const char* st, va_list & variableArgumentList);
};

#endif