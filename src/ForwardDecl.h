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
class Image;
class ImageLoader;
class InputHandler;
class Logger;
class Mesh;
class MeshLoader;
class OpenGLExtensionLoader;
class ResourceManager;
class Shader;
class ShaderLoader;
class Timer;
class VarGroup;

typedef std::shared_ptr<IFile> FilePtr;
typedef std::shared_ptr<Image> ImagePtr;
typedef std::shared_ptr<Mesh> MeshPtr;
typedef std::shared_ptr<Shader> ShaderPtr;
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