#include "Precomp.h"

#include "PNGLoader.h"
#include "stb_image.h"

#include "application/AppContext.h"
#include "utility/Logger.h"

png_loader::png_loader()
{
	//ctor
}

png_loader::~png_loader()
{
	//dtor
}

ImagePtr png_loader::Load(void * buffer, const uint32_t size)
{
	int32_t x = 0, y = 0, comp = 0;
	uint8_t * data = stbi_load_from_memory((const uint8_t*)buffer, size, &x, &y, &comp, 0);

	GetContext().GetLogger()->log(LOG_LOG, "Image loaded: w=%u; h=%u; comp=%u;", x, y, comp);

	if (data)
	{
		Image * image = new Image();
		image->data = data;
		image->num_channels = comp;
		image->width = x;
		image->height = y;
		return ImagePtr(image);
	}

	return ImagePtr(nullptr);
}

bool png_loader::CheckByExtension(const std::string & ext)
{
	return ext == "png" || ext == ".png" || ext == "jpg" || ext == ".jpg";
}