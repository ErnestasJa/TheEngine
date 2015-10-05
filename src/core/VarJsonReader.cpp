#include "Precomp.h"
#include "VarJsonReader.h"
#include "json/json.h"
#include "json/reader.h"
#include "core/Var.h"
#include "core/VarGroup.h"
#include "utility/Logger.h"


VarJsonReader::VarJsonReader(Logger * l)
{
	m_logger = l;
}

VarJsonReader::~VarJsonReader()
{
}

void LoadGroup(VarGroup & g, Json::Value & jg, Logger * logger);
bool VarJsonReader::Load(void * buffer, uint32_t size, VarGroup & group)
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

bool VarJsonReader::Load(const std::string & fileName, VarGroup & group)
{
	char * buf;
	uint32_t len;
	len=helpers::read(fileName,buf);

	if(len==0)
		return true;
	
	Load(buf, len, group);
	
	return true;
}

void BuildGroup(VarGroup & group, Json::Value & parentValue);
void WriteFile(const std::string & fileName, const std::string & jsonString, Logger * logger);

bool VarJsonReader::Write(const std::string & fileName, VarGroup & group)
{
	Json::StyledWriter writer;

	Json::Value root = Json::Value(Json::objectValue);
	
	BuildGroup(group, root);

	std::string buf = writer.write(root);

	WriteFile(fileName, buf, m_logger);
}

void WriteFile(const std::string & fileName, const std::string & jsonString, Logger * logger)
{
	PHYSFS_file* f = PHYSFS_openWrite(fileName.c_str());

	if(!f)
	{
		logger->log(LOG_LOG, "PHYSFS: Opening (%s) to write failed.\n", fileName.c_str());
		return;
	}

	PHYSFS_write(f, (void*)&jsonString[0], jsonString.size(), 1);
	PHYSFS_close(f);
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