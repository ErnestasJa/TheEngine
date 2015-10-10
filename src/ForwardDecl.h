#ifndef FORWARD_DECL_H
#define FORWARD_DECL_H

class VarGroup;
class Logger;
class FileSystem;
class IFile;
class AppContext;

namespace std
{
	template <class T>
	class shared_ptr;
}

typedef std::shared_ptr<IFile> FilePtr;

namespace boost
{
	namespace filesystem
	{
		class path;
	}
}

using Path = boost::filesystem::path;

#endif