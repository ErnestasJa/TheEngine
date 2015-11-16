#pragma once

#include "IImageLoader.h"
#include "ResourceCache.h"
#include "application/AppContext.h"

class AppContext;
class ImageLoader : public ResourceCache<Image>
{
protected:
	vector<IImageLoader*> m_loaders;
public:
	ImageLoader();
	virtual ~ImageLoader();

	virtual void AddLoader(IImageLoader * loader);
	virtual ImagePtr Load(const Path & file);
};
