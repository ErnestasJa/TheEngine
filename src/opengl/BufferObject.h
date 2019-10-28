#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include "IBufferObject.h"
#include "utility/Vector.h"

#define RegisterBufferObject(type, gltype, components) \
template <> \
uint32_t BufferObject<type>::GetDataType() \
{ \
	return gltype; \
} \
template <> \
uint32_t BufferObject<type>::GetComponentCount() \
{ \
	return components; \
} \
template <> \
void BufferObject<type>::Init() \
{ \
	glGenBuffers(1, &this->Id); \
} \
template <> \
void BufferObject<type>::Upload() \
{ \
	glBindBuffer(GL_ARRAY_BUFFER, this->Id); \
	glBufferData(GL_ARRAY_BUFFER, GetSize() * sizeof(type), data.size() > 0 ? &data[0] : 0, this->UsageHint); \
} \
template <> \
void BufferObject<type>::UploadSubData(vector<type> subdata, uint32_t offset) \
{ \
	glBindBuffer(GL_ARRAY_BUFFER, this->Id); \
	glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(type), subdata.size() * sizeof(type), &subdata[0]); \
}

template <class T>
struct BufferObject : public IBufferObject
{
	uint32_t preallocatedSize;
	vector<T> data;

	BufferObject(IBufferObject::USAGE_HINT hint, uint32_t prealloc=0) : IBufferObject(hint), preallocatedSize(prealloc)
	{

	}

	virtual ~BufferObject()
	{
		data.clear();
	}

	//void Init_buffers();
	virtual void Init();
	virtual void Upload();
	virtual void UploadSubData(vector<T> subdata, uint32_t offset);
	virtual void Clear()
	{
		data.clear();
	}

	virtual IBufferObject::IBO_TYPE GetType()
	{
		return IBufferObject::DATA;
	}
	virtual uint32_t GetDataType();
	virtual uint32_t GetComponentCount();
	virtual uint32_t GetSize()
	{
		return (uint32_t)(data.size() > 0 ? data.size() : (data.capacity() > 0 ? data.capacity() : preallocatedSize));
	}
};


#define RegisterIndexBufferObject(type, gltype, components) \
template <> \
uint32_t IndexBufferObject<type>::GetDataType() \
{ \
	return gltype; \
} \
template <> \
uint32_t IndexBufferObject<type>::GetComponentCount() \
{ \
	return components; \
} \
template <> \
void IndexBufferObject<type>::Init() \
{ \
	glGenBuffers(1, &this->Id); \
} \
template <> \
void IndexBufferObject<type>::Upload() \
{ \
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id); \
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetSize() * sizeof(type), data.size() > 0 ? &data[0] : 0, this->UsageHint); \
} \
template <> \
void IndexBufferObject<type>::UploadSubData(vector<type> subdata, uint32_t offset) \
{ \
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id); \
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(type), subdata.size() * sizeof(type), &subdata[0]); \
}

template <class T>
struct IndexBufferObject : public IBufferObject
{
	uint32_t preallocatedSize;
	vector<T> data;

	IndexBufferObject(IBufferObject::USAGE_HINT hint, uint32_t prealloc = 0) : IBufferObject(hint), preallocatedSize(prealloc)
	{
	}

	virtual ~IndexBufferObject()
	{
		data.clear();
	}

	//void Init_buffers();
	virtual void Init();
	virtual void Upload();
	virtual void UploadSubData(vector<T> subdata, uint32_t offset);
	virtual void Clear()
	{
		data.clear();
	}

	virtual IBufferObject::IBO_TYPE GetType()
	{
		return IBufferObject::INDEX;
	}
	virtual uint32_t GetDataType();
	virtual uint32_t GetComponentCount();
	virtual uint32_t GetSize()
	{
		return (uint32_t)(data.size() > 0 ? data.size() : (data.capacity() > 0 ? data.capacity() : preallocatedSize));
	}
};

#endif // BUFFER_OBJECT_H
