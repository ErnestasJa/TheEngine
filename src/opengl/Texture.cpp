#include "Precomp.h"
#include "resources/Image.h"
#include "Texture.h"

uint32_t Texture::active_slot = 0;
uint32_t Texture::current = 0;

Texture::Texture()
{
	Id = -1;
	Type = GL_TEXTURE_2D;
	dataType = GL_UNSIGNED_BYTE;
	internalFormat = GL_RGB;
	imageFormat = GL_RGB;
}

Texture::~Texture()
{
	glDeleteTextures(1, &Id);
}

void Texture::Init(ImagePtr img)
{
	glGenTextures(1, &Id);
	glBindTexture(Type, Id);

	glTexParameteri(Type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(Type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

	/// either use Texture with or without alpha channel
	switch (img->num_channels)
	{
	case 3:
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		break;

	case 4:
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		break;

	default:
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}

	dataType = GL_UNSIGNED_BYTE;
	imageFormat = img->num_channels == 4 ? GL_RGBA : GL_RGB;
	internalFormat = GL_RGBA;

	glTexImage2D(Type, 0, internalFormat, img->width, img->height, 0, imageFormat, dataType, img->data);

	glGenerateMipmap(Type);

	glBindTexture(Type, current);
}

/**
 * @brief
 * @param clamp
 * bit0 = s to edge, bit1 = t to edge
 * @param filter
 * bit0 = linear, bit1 = mipmap
 */
void Texture::Init(const uint8_t * data, uint32_t target, uint32_t image_format, uint32_t internal_format, int32_t w, int32_t h)
{
	Type = target;

	imageFormat = image_format;
	internalFormat = internal_format;

	glGenTextures(1, &Id);
	glBindTexture(Type, Id);

	glTexParameteri(Type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(Type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

	/// either use Texture with or without alpha channel
	switch (image_format)
	{
	case GL_RED:
	case GL_RGB:
	case GL_BGR:
		glPixelStorei(GL_UNPACK_ALIGNMENT, (uint32_t)TextureUnpackAlignment::BYTE);
		break;

	case GL_RG:
		glPixelStorei(GL_UNPACK_ALIGNMENT, (uint32_t)TextureUnpackAlignment::EVEN_BYTE);
		break;

	case GL_DEPTH_COMPONENT:
		glPixelStorei(GL_UNPACK_ALIGNMENT, (uint32_t)TextureUnpackAlignment::BYTE);
		break;

	default:
		glPixelStorei(GL_UNPACK_ALIGNMENT, (uint32_t)TextureUnpackAlignment::WORD);
		break;
	}

	switch (internalFormat)
	{
	case GL_RGBA32F:
	case GL_DEPTH_COMPONENT:
	case GL_DEPTH_COMPONENT16:
	case GL_DEPTH_COMPONENT24:
	case GL_DEPTH_COMPONENT32:
	case GL_DEPTH_COMPONENT32F:
		dataType = GL_FLOAT;
		break;
	default:
		dataType = GL_UNSIGNED_BYTE;
	}

	glTexImage2D(Type, 0, internalFormat, w, h, 0, imageFormat, dataType, data);

	//glinitMipmap(Type);

	glBindTexture(Type, current);
}

void Texture::SetFilters(TextureFilterMin fmin, TextureFilterMag fmag)
{
	if (current != Id)
		glBindTexture(Type, Id);

	glTexParameteri(Type, GL_TEXTURE_MAG_FILTER, fmag == TextureFilterMag::LINEAR ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(Type, GL_TEXTURE_MIN_FILTER, fmin == TextureFilterMin::LINEAR_MIPMAP ? GL_LINEAR_MIPMAP_LINEAR : (fmin == TextureFilterMin::NEAREST_MIPMAP ? GL_NEAREST_MIPMAP_NEAREST : (fmin == TextureFilterMin::LINEAR ? GL_LINEAR : GL_NEAREST)));

	if (current != Id)
		glBindTexture(Type, current);
}

void Texture::SetClampMode(TextureClamp x, TextureClamp y)
{
	if (current != Id)
		glBindTexture(Type, Id);

	glTexParameteri(Type, GL_TEXTURE_WRAP_S, x == TextureClamp::EDGE ? GL_CLAMP_TO_EDGE : (x == TextureClamp::BORDER ? GL_CLAMP_TO_BORDER : GL_REPEAT));
	glTexParameteri(Type, GL_TEXTURE_WRAP_T, y == TextureClamp::EDGE ? GL_CLAMP_TO_EDGE : (y == TextureClamp::BORDER ? GL_CLAMP_TO_BORDER : GL_REPEAT));

	if (current != Id)
		glBindTexture(Type, current);
}

void Texture::SetBorderColor(const glm::vec4 & color)
{
	if (current != Id)
		glBindTexture(Type, Id);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &color[0]);

	if (current != Id)
		glBindTexture(Type, current);
}

void Texture::SetUnpackAlignment(TextureUnpackAlignment alignment)
{
	if (current != Id)
		glBindTexture(Type, Id);

	glPixelStorei(GL_UNPACK_ALIGNMENT, (uint32_t)alignment);

	if (current != Id)
		glBindTexture(Type, current);
}

void Texture::InitMipmap()
{
	if (current != Id)
		glBindTexture(Type, Id);

	glGenerateMipmap(Type);

	if (current != Id)
		glBindTexture(Type, current);
}

void Texture::InitMipmap(uint32_t base, uint32_t max)
{
	if (current != Id)
		glBindTexture(Type, Id);

	glTexParameteri(Type, GL_TEXTURE_BASE_LEVEL, base);
	glTexParameteri(Type, GL_TEXTURE_MAX_LEVEL, max);
	glGenerateMipmap(Type);

	if (current != Id)
		glBindTexture(Type, current);
}

void Texture::UpdateMipmaps()
{
	if (current != Id)
		glBindTexture(Type, Id);

	glGenerateMipmap(Type);

	if (current != Id)
		glBindTexture(Type, current);
}

void Texture::Set(uint8_t slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(Type, Id);

	active_slot = slot;
	current = Id;
}

void Texture::SetSubImage2D(const uint8_t * data, uint32_t xoffset, uint32_t yoffset, uint32_t width, uint32_t height, uint32_t overrideFormat)
{
	glActiveTexture(GL_TEXTURE0 + active_slot);
	if (current != Id)
		glBindTexture(Type, Id);

	glTexSubImage2D(Type, 0, xoffset, yoffset, width, height, overrideFormat == 0 ? imageFormat : overrideFormat, dataType, data);

	if (current != Id)
		glBindTexture(Type, current);
}

void Texture::Unset(uint8_t slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(Type, 0);

	active_slot = slot;
	current = Id;
}

void Texture::Free()
{
}

GLO_TYPE Texture::GetType()
{
	return GLO_TEXTURE;
}