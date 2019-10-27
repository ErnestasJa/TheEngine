#ifndef IBUFFER_OBJECT_H
#define IBUFFER_OBJECT_H

struct IBufferObject
{
	enum IBO_TYPE
	{
		DATA = 0,
		INDEX
	};

	enum USAGE_HINT
	{
		STATIC = GL_STATIC_DRAW,
		DYNAMIC = GL_DYNAMIC_DRAW,
		STREAM = GL_STREAM_DRAW
	};

	uint32_t Id;
	uint32_t UsageHint;
	//bool enabled;

	IBufferObject(USAGE_HINT hint);
	virtual ~IBufferObject();

	virtual void Init() = 0;
	virtual void Upload() = 0;
	virtual void Clear() = 0;

	virtual IBufferObject::IBO_TYPE GetType() = 0;
	virtual uint32_t GetDataType() = 0;
	virtual uint32_t GetComponentCount() = 0;
	virtual uint32_t GetSize() = 0;
};

#endif // IBUFFER_OBJECT_H
