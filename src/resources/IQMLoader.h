#pragma once

#include "IMeshLoader.h"
#include "application/AppContext.h"

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;
struct iqmheader;
class Logger;

class IQMLoader : public IMeshLoader
{
private:
	void load_header(const char* data, iqmheader & header);
	void loadiqmanims(std::shared_ptr<Mesh> m, const char* data, iqmheader & header);
	Logger * m_logger;
public:
	IQMLoader();
	virtual ~IQMLoader();

	virtual MeshPtr Load(const char* data, const uint32_t size);
	virtual bool CheckByExtension(const std::string & ext);

private:
};
