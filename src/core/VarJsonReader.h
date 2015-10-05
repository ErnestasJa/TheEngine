#ifndef VARJSONREADER_H_INCLUDED
#define VARJSONREADER_H_INCLUDED

class VarGroup;
class Logger;
class VarJsonReader
{
public:
	VarJsonReader(Logger * l);
	~VarJsonReader();

	bool Read(void * buffer, uint32_t size, VarGroup & group);
	bool Read(const std::string & fileName, VarGroup & group);
	bool Write(const std::string & fileName, VarGroup & group);

private:
	Logger * m_logger;
};

#endif // VARJSONREADER_H_INCLUDED
