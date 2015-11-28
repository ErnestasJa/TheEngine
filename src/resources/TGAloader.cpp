#include "Precomp.h"

#include "TGAloader.h"
#include "glm.hpp"

#include "application/AppContext.h"
#include "modules/logging/Logger.h"

tgaloader::tgaloader()
{
}

tgaloader::~tgaloader()
{
	//dtor
}

ImagePtr tgaloader::Load(void * buffer, const uint32_t size)
{
	memcpy(&m_header, buffer, this->getHeaderSize());

	if (m_header.datatypecode == 2)/// load the uncompressed TGA
		return ImagePtr(this->loadUncompressedTGA(buffer, size));
	else
		GetContext().GetLogger()->log(LOG_WARN, "bad tga header: %i\n", (int)m_header.datatypecode);

	return ImagePtr(NULL);
}

Image * tgaloader::loadUncompressedTGA(void * buffer, const uint32_t size)
{
	//memcpy(&tga.header, buffer, this->getHeaderSize());
	uint8_t components = m_header.bitsperpixel / 8;
	if (m_header.width <= 0 || m_header.height <= 0 || (components != 3 && components != 4))
	{
		GetContext().GetLogger()->log(LOG_LOG, "Ooops, something is wrong with image: %i, %i, %i\n", (int)m_header.width, (int)m_header.height, (int)m_header.bitsperpixel);
		return 0;
	}

	uint8_t * data = new uint8_t[m_header.width * m_header.height * components];

	memcpy(data, &static_cast<uint8_t*>(buffer)[this->getHeaderSize()], m_header.width*m_header.height*components);

	uint8_t tmp;

	for (uint32_t i = 0; i < (uint32_t)m_header.width*m_header.height*components; i += components)
	{
		tmp = data[i];
		data[i] = data[i + 2];
		data[i + 2] = tmp;
	}

	if (data)
	{
		Image * image = new Image();
		image->data = data;
		image->num_channels = components;
		image->width = m_header.width;
		image->height = m_header.height;
		return image;
	}

	return nullptr;
}

bool tgaloader::CheckByExtension(const std::string & ext)
{
	return ext == "tga" || ext == ".tga";
}

bool tgaloader::checkByHeader(void * hbuffer, const uint32_t hsize)
{
	return true;
}

uint32_t tgaloader::getHeaderSize()
{
	return sizeof(HEADER);
}