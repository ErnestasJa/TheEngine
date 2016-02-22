#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include "IBufferObject.h"
#include "utility/Vector.h"

template <class T>
struct BufferObject : public IBufferObject
{
	uint32_t preallocatedSize;
	vector<T> data;

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

template <class T>
struct IndexBufferObject : public IBufferObject
{
	uint32_t preallocatedSize;
	vector<T> data;

	virtual ~IndexBufferObject()
	{
		data.clear();
	}

	//void Init_buffers();
	virtual void Init();
	virtual void Upload();
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
