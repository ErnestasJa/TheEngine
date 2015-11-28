#include "Precomp.h"
#include "RenderObjectManager.h"
#include "RenderStateManager.h"
#include "application/AppContext.h"
#include "modules/logging/Logger.h"

RenderaObjectManager::RenderaObjectManager()
{

	
}

RenderaObjectManager::~RenderaObjectManager()
{

}

void RenderaObjectManager::RenderObjects(const vector<IRenderObject*> & renderObjects)
{
	for (IRenderObject * object : renderObjects)
	{

	}
}