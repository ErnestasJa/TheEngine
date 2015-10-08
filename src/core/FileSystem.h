#ifndef FILESYSTEM
#define FILESYSTEM

#include "utility/vector.h"

using Path = boost::filesystem::path;
class File;
struct AppContext;

class FileSystem
{
public:
	FileSystem(AppContext * appContext);
	virtual ~FileSystem();

	bool SetWriteDirectory(const Path & path);
	Path GetWriteDirectory();
	Path GetWorkingDirectory();
	bool AddSearchDirectory(const Path & path);

	//File OpenWrite(const Path & path);
	File OpenRead(const Path & path);

private:	
	AppContext * m_appContext;
	Path m_writeDirectory, m_workingDirectory;
	vector<Path> m_searchDirectories;
};

#endif