#include "Precomp.h"

#include "ImageLoader.h"
#include "PNGLoader.h"
#include "TGAloader.h"
#include "opengl/Texture.h"

#include "utility/Logger.h"
#include "core/FileSystem.h"

ImageLoader::ImageLoader()
{
	AddLoader(new png_loader());
	AddLoader(new tgaloader());
}

ImageLoader::~ImageLoader()
{
	for (IImageLoader * l : m_loaders)
		delete l;
}

void ImageLoader::AddLoader(IImageLoader * loader)
{
	auto it = std::find_if(m_loaders.begin(), m_loaders.end(), [&loader](IImageLoader * l)
	{
		return l == loader;
	});

	if (it == m_loaders.end())
		m_loaders.push_back(loader);
}

ImagePtr ImageLoader::Load(const Path & filePath)
{
	auto res = this->get_resource(filePath);

	if (res._resource)
	{
		GetContext().GetLogger()->log(LOG_LOG, "Found image in cache, skipping loading.");
		return res._resource;
	}

	std::string ext = filePath.extension().generic_string();
	GetContext().GetLogger()->log(LOG_LOG, "Image extension: '%s'", ext.c_str());
	auto fileSystem = GetContext().GetFileSystem();

	if (fileSystem->OpenRead(filePath))
	{
		for (IImageLoader * l : m_loaders)
		{
			if (l->CheckByExtension(ext))
			{
				FilePtr file = fileSystem->OpenRead(filePath);

				if(file)
				{
					ByteBufferPtr buffer = file->Read();

					if (buffer->size() != 0)
					{
						GetContext().GetLogger()->log(LOG_LOG, "Image file size: %u", buffer->size());

						res._path = filePath;
						res._resource = l->Load(buffer->data(), buffer->size());
						this->add_resource(res);
						return res._resource;
					}
				}
			}
		}
	}

	return nullptr;
}