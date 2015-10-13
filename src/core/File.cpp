#include "Precomp.h"

#include "boost/filesystem/operations.hpp"

#include "File.h"

File::File(const Path & file, EFileMode mode)
{
	m_mode = mode;
	auto file_cstr = file.generic_string().c_str();
	if( mode & EFM_READ )
	{
			if(mode & EFM_APPEND)
			{
				m_fileHandle = PHYSFS_openAppend(file_cstr);
				
				if(m_fileHandle == nullptr)
				{
					printf("File open append failed: %s\n", PHYSFS_getLastError());
				}
			}
			else
			{
				m_fileHandle = PHYSFS_openRead(file_cstr);
				
				if(m_fileHandle == nullptr)
				{
					printf("File open read failed: %s\n", PHYSFS_getLastError());
				}
			}
	}
	else if( mode & EFM_WRITE )
	{
		m_fileHandle = PHYSFS_openWrite(file_cstr);

		if(m_fileHandle == nullptr)
		{
			printf("File open write failed: %s\n", PHYSFS_getLastError());
		}

	}
	else
		m_fileHandle = nullptr;
}

File::~File()
{
	if(m_fileHandle)
	{
		if(m_mode&EFM_WRITE)
			PHYSFS_flush(m_fileHandle);
		
		PHYSFS_close(m_fileHandle);
	}
}


bool File::IsOpen()
{
	return m_fileHandle != nullptr;
}

EFileMode File::GetMode()
{
	return m_mode;
}

uint32_t File::GetLength()
{
	if(m_fileHandle){
		return PHYSFS_fileLength(m_fileHandle);
	}
	else
		return 0;
}

ByteBufferPtr File::Read()
{
	uint32_t length = GetLength();
	return Read(length);
}

ByteBufferPtr File::Read(uint32_t length)
{
	if(m_fileHandle)
	{
		ByteBuffer * buffer = new ByteBuffer();
		buffer->reserve(length);

		uint32_t bytesRead = PHYSFS_read(m_fileHandle, (char *)buffer->data(), length, 1) * length;

		buffer->resize(bytesRead);
	
		return share(buffer);
	}
	else
	{
		return std::shared_ptr<ByteBuffer>();
	}
}


uint32_t File::Write(void * buffer, uint32_t length)
{
	if(m_fileHandle)
	{
		return PHYSFS_write(m_fileHandle, (char *)buffer, length, 1) * length;
	}
	else
	{
		return 0;
	}
}