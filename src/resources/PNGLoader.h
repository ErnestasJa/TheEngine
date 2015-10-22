#pragma once

#include "IImageLoader.h"
// FIXME (Ernestas#1#): some pngs are not loaded, make sure they don't crash app

class png_loader : public IImageLoader
{
public:
	png_loader();
	virtual ~png_loader();

	virtual ImagePtr Load(void * buffer, const uint32_t size);
	virtual bool CheckByExtension(const std::string & ext);

protected:
};
