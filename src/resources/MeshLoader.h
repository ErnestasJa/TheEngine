#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include "IMeshLoader.h"
#include "ResourceCache.h"
#include "ForwardDecl.h"

class AppContext;
class mesh_loader : public resource_cache<Mesh>
{
public:
	mesh_loader(AppContext * appContext);
	virtual ~mesh_loader();

	virtual void add_loader(imesh_loader * loader);
	virtual MeshPtr load(const Path & file);

protected:
	AppContext * m_appContext;
	vector<imesh_loader*> m_loaders;
};

#endif // MESH_LOADER_H
