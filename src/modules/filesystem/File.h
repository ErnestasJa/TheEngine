#ifndef FILE_H
#define FILE_H

#include "IFile.h"
#include "boost/filesystem/path.hpp"

#ifdef _WIN32
#pragma message(": warning FS01: this should be predefined somewhere in single include.")
#else
#warning "should be predefined somewhere in single include."
#endif

using Path = boost::filesystem::path;

class File : public IFile
{
public:
	File(const Path & file, EFileMode mode);
	virtual ~File();
	virtual bool IsOpen();
	virtual EFileMode GetMode();
	virtual uint32_t GetLength();
	virtual ByteBufferPtr Read();
	virtual ByteBufferPtr Read(uint32_t length);
	virtual ByteBufferPtr ReadText();
	virtual ByteBufferPtr ReadText(uint32_t length);
	virtual uint32_t Write(void * buffer, uint32_t length);
private:
	PHYSFS_file * m_fileHandle;
	EFileMode m_mode;
};

#endif