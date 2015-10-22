#ifndef IMESH_LOADER_H
#define IMESH_LOADER_H

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;
class IMeshLoader
{
public:
	virtual ~IMeshLoader();

	virtual MeshPtr Load(const char* data, const uint32_t size) = 0;
	virtual bool CheckByExtension(const std::string & ext) = 0;
protected:
private:
};

#endif // IMESH_LOADER_H
