#ifndef RENDERABLE_OBJECT_MANAGER_H
#define RENDERABLE_OBJECT_MANAGER_H

#include "core/Vector.h"
#include "ForwardDecl.h"

/*
Render object manager does not hold any list of render objects.
Because those objects already exist on user code most likelly in a vector, why duplicate and sync that?.
*/

class RenderaObjectManager
{
public:
	RenderaObjectManager();
	virtual ~RenderaObjectManager();

	void RenderObjects(const vector<IRenderObject*> & renderObjects);
};

#endif