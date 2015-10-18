#include "Precomp.h"

#include "application/AppContext.h"
#include "utility/Logger.h"
#include "FileSystem.h"
#include "File.h"

#include "boost/filesystem/operations.hpp"

Path GetPhysFSWorkingDirectory();

FileSystem::FileSystem(const char * argv)
{
	PHYSFS_init(argv);
	m_workingDirectory = GetPhysFSWorkingDirectory();
	PHYSFS_mount(m_workingDirectory.generic_string().c_str(), NULL, 0);
}

FileSystem::~FileSystem()
{
	if(!PHYSFS_deinit())
		std::cout << "PHYSFS_deinit() failed!\n reason: " << PHYSFS_getLastError() << "." << std::endl;
}

bool FileSystem::SetWriteDirectory(const Path & path)
{
	int32_t status = PHYSFS_setWriteDir(path.c_str());

	if (status == 0)
	{
		return false;
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

FilePtr FileSystem::OpenRead(const Path & path)
{
	IFile * file = new File(path, EFM_READ);
	return share(file);
}

FilePtr FileSystem::OpenWrite(const Path & path)
{
	IFile * file = new File(path, EFM_WRITE);
	return share(file);
}

bool FileSystem::DirectoryExists(const Path & path)
{
	return PHYSFS_isDirectory(path.generic_string().c_str()) != 0;
}

bool FileSystem::FileExists(const Path & path)
{
	return PHYSFS_exists(path.generic_string().c_str()) != 0;
}

bool FileSystem::CreateDirectory(const Path & path)
{
	int returnCode  = PHYSFS_mkdir(path.generic_string().c_str()) != 0;

	if(returnCode == 0)
	{
		GetContext().GetLogger()->log(LOG_LOG, "Failed to create directory, error: %s", PHYSFS_getLastError());
		//printf("Failed to create directory, error: %s\n", PHYSFS_getLastError());
	}

	return returnCode != 0;
}


///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`
Path GetPhysFSWorkingDirectory()
{
	std::string workingDirStr = PHYSFS_getBaseDir();
	Path workingDirectory(workingDirStr);

	if (workingDirectory.has_filename())
		workingDirectory = workingDirectory.parent_path();

	return workingDirectory;
}