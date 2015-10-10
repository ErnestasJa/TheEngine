#include "Precomp.h"
#include "VarJsonReader.h"
#include "json/json.h"
#include "json/reader.h"
#include "core/Var.h"
#include "core/VarGroup.h"
#include "utility/Logger.h"
#include "application/AppContext.h"


VarJsonReader::VarJsonReader(AppContext * appContext)
{
	m_appContext = appContext;
	m_logger = appContext->logger;
	m_fileSystem = appContext->fileSystem;
}

VarJsonReader::~VarJsonReader()
{
}

void LoadGroup(VarGroup & g, Json::Value & jg, Logger * logger);
bool VarJsonReader::Read(void * buffer, uint32_t size, VarGroup & group)
{
	Json::Reader reader;
	const char * doc = (const char *)buffer;

	Json::Value root;
	reader.parse(doc, doc + size, root);

	if (m_logger)
		m_logger->log(LOG_LOG, "Group: %s", group.Name());

	LoadGroup(group, root, m_logger);
	return true;
}

bool VarJsonReader::Read(const Path & fileName, VarGroup & group)
{
	FilePtr file = m_fileSystem->OpenRead(fileName);
	
	if(file && file->IsOpen())
	{
		ByteBufferPtr buffer = file->Read();
		return Read(buffer->data(), buffer->size(), group);
	}

	return false;
}

void BuildGroup(VarGroup & group, Json::Value & parentValue);
void WriteFile(const Path & fileName, const std::string & jsonString, AppContext * appContext);

bool VarJsonReader::Write(const Path & fileName, VarGroup & group)
{
	Json::StyledWriter writer;

	Json::Value root = Json::Value(Json::objectValue);
	
	BuildGroup(group, root);

	std::string buf = writer.write(root);

	WriteFile(fileName, buf, m_appContext);

	return true;
}

void WriteFile(const Path & fileName, const std::string & jsonString, AppContext * appContext)
{
	FilePtr file = appContext->fileSystem->OpenWrite(fileName);
	
	if(file && file->IsOpen())
	{
		file->Write((void*)&jsonString[0], jsonString.size());
	}
}

void BuildVars(VarGroup & group, Json::Value & groupValue);
void BuildGroup(VarGroup & group, Json::Value & parentValue)
{
	Json::Value newGroup = Json::Value(Json::objectValue);
	parentValue[group.Name()] = newGroup;

	BuildVars(group, newGroup);

	for(auto varGroup : group.Groups())
	{
		BuildGroup(varGroup, newGroup);
	}
}


void BuildVars(VarGroup & group, Json::Value & groupValue)
{
	for(auto & var : group.Vars())
	{
		switch(var.Type()){
			case VARI:
				groupValue[var.Name()] = Json::Value(var.ValueI());
			break;

			case VARF:
				groupValue[var.Name()] = Json::Value(var.ValueF());
			break;

			case VARS:
				groupValue[var.Name()] = Json::Value(var.ValueS());
			break;

			default:
			break;
		}
	}
}


void LoadGroup(VarGroup & g, Json::Value & jg, Logger * logger)
{
	Json::Value jgroup = jg[g.Name()];

	if (jgroup.isNull() || !jgroup.isObject())
		return;

	if (logger)
		logger->log(LOG_LOG, "Loading group: %s", g.Name());

	for (Var & var : g.Vars())
	{
		Json::Value jvar = jgroup[var.Name()];

		if (!jvar.isNull())
			switch (var.Type())
			{
			case VARI:
				var.Value(jvar.asInt());
				break;
			case VARF:
				var.Value(jvar.asFloat());
				break;
			case VARS:
				var.Value(jvar.asString().c_str());
				break;
			default:
				break;
			}
	}

	for (VarGroup & group : g.Groups())
		LoadGroup(group, jgroup, logger);
}