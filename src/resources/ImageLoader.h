#pragma once

#include "IImageLoader.h"
#include "ResourceCache.h"
#include "application/AppContext.h"

struct Texture;
typedef std::shared_ptr<Texture> TexturePtr;
class AppContext;
class image_loader : public resource_cache<image>
{
protected:
	AppContext * m_appContext;
	vector<iimage_loader*> m_loaders;
public:
	image_loader(AppContext * appContext);
	virtual ~image_loader();

	virtual void add_loader(iimage_loader * loader);
	virtual image_ptr load(const Path & file);
};
