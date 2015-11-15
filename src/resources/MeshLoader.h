#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include "IMeshLoader.h"
#include "ResourceCache.h"
#include "ForwardDecl.h"

class MeshLoader : public ResourceCache<Mesh>
{
public:
	MeshLoader();
	virtual ~MeshLoader();

	virtual void AddLoader(IMeshLoader * loader);
	virtual MeshPtr Load(const Path & file);

protected:
	vector<IMeshLoader*> m_loaders;
};

#endif // MESH_LOADER_H
