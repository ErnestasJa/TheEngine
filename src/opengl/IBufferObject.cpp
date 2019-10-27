#include "Precomp.h"
#include "IBufferObject.h"

IBufferObject::IBufferObject(USAGE_HINT hint) : Id(0), UsageHint(hint)
{
}

IBufferObject::~IBufferObject()
{
	if (Id != 0)
	{
		glDeleteBuffers(1, &Id);
	}
}