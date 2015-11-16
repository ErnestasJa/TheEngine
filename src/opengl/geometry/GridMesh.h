#ifndef GRIDMESH_H_INCLUDED
#define GRIDMESH_H_INCLUDED

#include "../Mesh.h"

class GridMesh : public Mesh
{
public:
	GridMesh(float density, uint32_t size = 256, uint32_t gridlines = 16, bool positiveOnly = false);
	~GridMesh();
private:
protected:
};

#endif // GRIDMESH_H_INCLUDED
