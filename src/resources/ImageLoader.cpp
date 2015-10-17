#include "Precomp.h"

#include "ImageLoader.h"
#include "PNGLoader.h"
#include "TGAloader.h"
#include "opengl/Texture.h"

#include "utility/Logger.h"

image_loader::image_loader(AppContext * appContext) : m_appContext(appContext)
{
	add_loader(new png_loader(appContext));
	add_loader(new tgaloader(appContext));
}

image_loader::~image_loader()
{
	for (iimage_loader * l : m_loaders)
		delete l;
}

void image_loader::add_loader(iimage_loader * loader)
{
	auto it = std::find_if(m_loaders.begin(), m_loaders.end(), [&loader](iimage_loader * l)
	{
		return l == loader;
	});

	if (it == m_loaders.end())
		m_loaders.push_back(loader);
}

image_ptr image_loader::load(const Path & filePath)
{
	resource<image> res;
	res = this->get_resource(filePath);

	if (res._resource)
	{
		m_appContext->logger->log(LOG_LOG, "Found image in cache, skipping loading.");
		return res._resource;
	}

	std::string ext = filePath.extension().generic_string();
	m_appContext->logger->log(LOG_LOG, "Image extension: '%s'", ext.c_str());
	auto fileSystem = m_appContext->fileSystem;

	if (fileSystem->OpenRead(filePath))
	{
		for (iimage_loader * l : m_loaders)
		{
			if (l->check_by_extension(ext))
			{
				FilePtr file = fileSystem->OpenRead(filePath);

				if(file)
				{
					ByteBufferPtr buffer = file->Read();

					if (buffer->size() != 0)
					{
						m_appContext->logger->log(LOG_LOG, "Image file size: %u", buffer->size());

						res._path = filePath;
						res._resource = image_ptr(l->load(buffer->data(), buffer->size()));
						this->add_resource(res);
						return res._resource;
					}
				}
			}
		}
	}

	return nullptr;
}