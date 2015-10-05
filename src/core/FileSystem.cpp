#include "Precomp.h"

#include "application/AppContext.h"
#include "utility/Logger.h"
#include "FileSystem.h"

#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"

Path GetWorkingDirectory();

FileSystem::FileSystem(AppContext * appContext, const char * argv)
{
	m_appContext = appContext;
	PHYSFS_init(argv);
	m_workingDirectory = GetWorkingDirectory();
	PHYSFS_mount(m_workingDirectory.generic_string().c_str(), NULL, 0);
}

bool FileSystem::SetWriteDirectory(const Path & path)
{
	int32_t status = PHYSFS_setWriteDir(path.c_str());

	if (status == 0)
	{
		return false
	}

	m_writeDirectory = path.generic_string();
	return true;
}

Path FileSystem::GetWriteDirectory()
{
	return m_writeDirectory;
}

Path FileSystem::GetWorkingDirectory()
{
	return m_workingDirectory;
}

bool FileSystem::AddSearchDirectory(const Path & path)
{
	int32_t status = PHYSFS_mount(path.generic_string().c_str(), NULL, 0);

	if (status == 0)
	{
		return false;
	}

	m_searchDirectories.push_back(path);
	return true;
}

File FileSystem::OpenWrite(const Path & path)
{

}

File FileSystem::OpenRead(const Path & path)
{

}


///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`
Path GetWorkingDirectory()
{
	std::string workingDirStr = PHYSFS_getBaseDir();
	Path wd(workingDirStr);

	if (wd.has_filename())
		wd = wd.parent();

	return wd;
}