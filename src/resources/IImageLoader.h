#pragma once

#include "Image.h"

class IImageLoader
{
public:
	IImageLoader();
	virtual ~IImageLoader();
	virtual ImagePtr Load(void * buffer, const uint32_t size) = 0;
	virtual bool CheckByExtension(const std::string & ext) = 0;
protected:
private:
};
