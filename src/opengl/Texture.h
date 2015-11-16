#pragma once

#include "ForwardDecl.h"
#include "GLObject.h"

enum class TextureFilterMin :uint32_t
{
	NEAREST = 0,
	LINEAR,
	LINEAR_MIPMAP,
	NEAREST_MIPMAP
};

enum class TextureFilterMag :uint32_t
{
	NEAREST = 0,
	LINEAR,
};

enum class TextureClamp :uint32_t
{
	REPEAT = 0,
	EDGE,
	BORDER
};

enum class TextureUnpackAlignment :uint32_t
{
	BYTE = 1,
	EVEN_BYTE = 2,
	WORD = 4,
	DOUBLE_WORD = 8
};

class Texture : public GLObject
{
public:
	std::string name;
	uint32_t Type;
	uint32_t imageFormat;
	uint32_t internalFormat;
	uint32_t dataType;

	static uint32_t current, active_slot;

	Texture();
	~Texture();

	void Init(ImagePtr img);
	void Init(const uint8_t * data, uint32_t target, uint32_t image_format, uint32_t internal_format, int32_t w, int32_t h);

	void SetFilters(TextureFilterMin fmin, TextureFilterMag fmag);
	void SetClampMode(TextureClamp x, TextureClamp y);
	void SetSubImage2D(const uint8_t * data, uint32_t xoffset, uint32_t yoffset, uint32_t width, uint32_t height, uint32_t overrideFormat = 0);
	void SetBorderColor(const glm::vec4 & color);
	void SetUnpackAlignment(TextureUnpackAlignment alignment);
	void InitMipmap(uint32_t base, uint32_t max);
	void InitMipmap();
	void UpdateMipmaps();
	void Set(uint8_t slot);
	void Unset(uint8_t slot);
	void Free();
	virtual GLO_TYPE GetType();
};

typedef std::shared_ptr<Texture> TexturePtr;
