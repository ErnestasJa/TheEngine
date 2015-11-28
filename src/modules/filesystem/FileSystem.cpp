#include "Precomp.h"

#include "application/AppContext.h"
#include "modules/logging/Logger.h"
#include "FileSystem.h"
#include "File.h"
#include "PathTools.h"

Path GetPhysFSWorkingDirectory();

FileSystem::FileSystem(const char * argv)
{
	std::string path = argv;

	path = WorkaroundVisualStudio(path);

	bool successInit = PHYSFS_init(path.c_str()) == 0;
	PHYSFS_permitSymbolicLinks(1);
	m_workingDirectory = GetPhysFSWorkingDirectory();
	auto test = m_workingDirectory.generic_string();
	auto cstr = test.c_str();
	PHYSFS_mount(cstr, NULL, 0);
}

FileSystem::~FileSystem()
{
	if (!PHYSFS_deinit())
		std::cout << "PHYSFS_deinit() failed!\n reason: " << PHYSFS_getLastError() << "." << std::endl;
}

bool FileSystem::Initialize(EngineModuleProviderWeakPtr ptr)
{
	return true;
}

bool FileSystem::IsInitialized() const
{
	return true;
}

const std::string & FileSystem::GetName() const
{
	return "File system";
}

const ModuleType FileSystem::GetType() const
{
	return ModuleType::FileSystem;
}

uint32_t FileSystem::GetExtendedType() const
{
	return 0;
}

bool FileSystem::SetWriteDirectory(const Path & path)
{
	auto posixPath = MakePosix(path);
	auto genericString = posixPath.generic_string();
	int32_t status = PHYSFS_setWriteDir(genericString.c_str());

	if (status == 0)
	{
		return false;
	}

	m_writeDirectory = posixPath.generic_string();
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
	auto posixPath = MakePosix(path);
	auto genericString = posixPath.generic_string();
	int32_t status = PHYSFS_mount(genericString.c_str(), NULL, 0);

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
	auto genericString = path.generic_string();
	return PHYSFS_isDirectory(genericString.c_str()) != 0;
}

bool FileSystem::FileExists(const Path & path)
{
	auto genericString = path.generic_string();
	return PHYSFS_exists(genericString.c_str()) != 0;
}

bool FileSystem::CreateDirectory(const Path & path)
{
	auto genericString = path.generic_string();
	int returnCode = PHYSFS_mkdir(genericString.c_str()) != 0;

	if (returnCode == 0)
	{
		GetContext().GetLogger()->log(LOG_LOG, "Failed to create directory, error: %s", PHYSFS_getLastError());
		//printf("Failed to create directory, error: %s\n", PHYSFS_getLastError());
	}
	

	return returnCode != 0;
}

void AppendFiles(void *data, const char *directory, const char * fileName);
vector<Path> FileSystem::GetFilesInDirectory(const Path & directory)
{
	vector<Path> paths;
	PHYSFS_enumerateFilesCallback(directory.generic_string().c_str(), AppendFiles, &paths);
	return paths;
}

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`

void AppendFiles(void *data, const char *directory, const char * fileName)
{
	Path path(directory);
	path.append(fileName);

	auto paths = static_cast<vector<Path>*>(data);
	paths->push_back(path);
}

Path GetPhysFSWorkingDirectory()
{
	std::string workingDirStr = PHYSFS_getBaseDir();
	Path workingDirectory(workingDirStr);

	if (workingDirectory.has_filename())
		workingDirectory = workingDirectory.parent_path();

	return workingDirectory;
}