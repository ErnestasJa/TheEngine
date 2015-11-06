#pragma once

#include "ForwardDecl.h"
#include "GLObject.h"

class Texture : public GLObject
{
public:
	std::string name;
	uint32_t Type;
	uint32_t imageFormat;
	uint32_t internalFormat;
	uint32_t dataType;

	static uint32_t current, active_slot;

	enum FILTER_MIN :uint32_t
	{
		FILTER_MIN_NEAREST = 0,
		FILTER_MIN_LINEAR,
		FILTER_MIN_LINEAR_MIPMAP,
		FILTER_MIN_NEAREST_MIPMAP
	};

	enum FILTER_MAG :uint32_t
	{
		FILTER_MAG_NEAREST = 0,
		FILTER_MAG_LINEAR,
	};

	enum CLAMP :uint32_t
	{
		CLAMP_REPEAT = 0,
		CLAMP_EDGE,
		CLAMP_BORDER
	};

	enum UNPACK_ALIGNMENT :uint32_t
	{
		UNPACK_BYTE_ALIGNMENT = 1,
		UNPACK_EVEN_BYTE_ALIGNMENT = 2,
		UNPACK_WORD_ALIGNMENT = 4,
		UNPACK_DOUBLE_WORD_ALIGNMENT = 8
	};

	Texture();
	~Texture();

	void Init(ImagePtr img);
	void Init(const uint8_t * data, uint32_t target, uint32_t image_format, uint32_t internal_format, int32_t w, int32_t h);

	void SetFilters(Texture::FILTER_MIN fmin, Texture::FILTER_MAG fmag);
	void SetClampMode(Texture::CLAMP x, Texture::CLAMP y);
	void SetSubImage2D(const uint8_t * data, uint32_t xoffset, uint32_t yoffset, uint32_t width, uint32_t height, uint32_t overrideFormat = 0);
	void SetBorderColor(const glm::vec4 & color);
	void SetUnpackAlignment(Texture::UNPACK_ALIGNMENT alignment);
	void InitMipmap(uint32_t base, uint32_t max);
	void InitMipmap();
	void UpdateMipmaps();
	void Set(uint8_t slot);
	void Unset(uint8_t slot);
	void Free();
	virtual GLO_TYPE GetType();
};

typedef std::shared_ptr<Texture> TexturePtr;
