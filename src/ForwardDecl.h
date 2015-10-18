#ifndef FORWARD_DECL_H
#define FORWARD_DECL_H

namespace std
{
	template <class T>
	class shared_ptr;
}

class AppContext;
class ApplicationSettingsManager;
class ApplicationWindow;
class FileSystem;
class GUIEnvironment;
class IFile;
class InputHandler;
class Logger;
class OpenGLUtil;
class Timer;
class VarGroup;

typedef std::shared_ptr<IFile> FilePtr;
typedef std::shared_ptr<Timer> TimerPtr;

namespace boost
{
	namespace filesystem
	{
		class path;
	}
}

using Path = boost::filesystem::path;

#endif