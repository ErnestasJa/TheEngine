#include "Precomp.h"

#include "modules/logging/Logger.h"
#include "application/AppContext.h"
#include "application/SettingsManager.h"
#include "utility/Timer.h"
#include "modules/filesystem/FileSystem.h"
#include "modules/EngineModuleProvider.h"


Logger::Logger(int verbosity)
{
	m_verbosity = verbosity;
	log(LOG_DEBUG, "Logger initialised...");
}

Logger::~Logger()
{
	log(LOG_DEBUG, "Logger is terminating...");
}

bool Logger::Initialize(EngineModuleProviderWeakPtr ptr)
{
	if (auto provider = ptr.lock())
	{
		auto modulew = provider->GetModuleWithType<FileSystem>(ModuleType::FileSystem);

		if(auto module = modulew.lock())
		{
			m_logfile = module->OpenWrite(GenerateLogFileName());
		}
	}

	return false;
}

bool Logger::IsInitialized() const
{
	return true;
}

const std::string & Logger::GetName() const
{
	return "Logging";
}

const ModuleType Logger::GetType() const
{
	return ModuleType::Logging;
}

uint32_t Logger::GetExtendedType() const
{
	return 0;
}


void Logger::log(loglevel lev, const char* formatString, ...)
{
	va_list variableArgumentList;
  	va_start(variableArgumentList, formatString);
	std::string logMessage = FormatMessage(lev, formatString, variableArgumentList);
	va_end(variableArgumentList);

	if(m_logfile)
		m_logfile->Write((void*)logMessage.c_str(), logMessage.size());
	
	printf("%s", logMessage.c_str());
}

void Logger::SetLogFile(FilePtr file)
{
	m_logfile = file;
}

std::string Logger::FormatMessage(loglevel lev, const char* formatString, va_list & variableArgumentList)
{
	char buffer[256];
	vsnprintf(buffer, 256, formatString, variableArgumentList);

	std::string message = GenerateTimestamp() + " ";

	switch (lev)
	{
	case LOG_LOG:
		message += "LOG: ";
		break;
	case LOG_DEBUG:
		message += "DEBUG: ";
		break;
	case LOG_WARN:
		message += "WARNING: ";
		break;
	case LOG_ERROR:
		message += "ERROR: ";
		break;
	case LOG_CRITICAL:
		message += "CRITICAL: ";
		break;
	default:
		break;
	}

	message.append(buffer);
	message.append("\n");

	return message;
}

std::string Logger::GenerateTimestamp()
{
	GetContext().GetTimer()->tick();
	std::string stamp = "";
	uint32_t t = GetContext().GetTimer()->get_time() / 1000;

	uint32_t h, m, s;

	h = t / 3600;
	m = t / 60;
	s = t - h * 3600 - m * 60;

	if (h < 10)
		stamp += "0";
	stamp += helpers::to_str(h);
	stamp += ":";
	if (m < 10)
		stamp += "0";
	stamp += helpers::to_str(m);
	stamp += ":";
	if (s < 10)
		stamp += "0";
	stamp += helpers::to_str(s);

	return stamp;
}

Path Logger::GenerateLogFileName()
{
	Path logPath = GetContext().GetApplicationSettingsManager()->GetGroup("filesystem").GetVar("log_path").ValueS();
	logPath.append(helpers::to_str(GetContext().GetTimer()->get_real_time()) + "_log.txt");
	return logPath;
}
