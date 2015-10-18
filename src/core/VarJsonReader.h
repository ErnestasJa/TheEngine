#ifndef VARJSONREADER_H_INCLUDED
#define VARJSONREADER_H_INCLUDED

#include "ForwardDecl.h"

class VarJsonReader
{
public:
	VarJsonReader();
	~VarJsonReader();

	bool Read(void * buffer, uint32_t size, VarGroup & group);
	bool Read(const Path & fileName, VarGroup & group);
	bool Write(const Path & fileName, VarGroup & group); //Yep, change class name or split it.

private:
	Logger * m_logger;
	FileSystem * m_fileSystem;
};

#endif // VARJSONREADER_H_INCLUDED
