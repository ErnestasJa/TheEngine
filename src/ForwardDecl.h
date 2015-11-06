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
class Camera;
class FileSystem;
class GUIEnvironment;
class IBindingValue;
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
class Material;
class Texture;

typedef std::shared_ptr<Camera> CameraPtr;
typedef std::shared_ptr<IFile> FilePtr;
typedef std::shared_ptr<IBindingValue> IBindingValuePtr;
typedef std::shared_ptr<Image> ImagePtr;
typedef std::shared_ptr<Material> MaterialPtr;
typedef std::shared_ptr<Mesh> MeshPtr;
typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<Shader> ShaderPtr;
typedef std::shared_ptr<Timer> TimerPtr;

typedef uint32_t GLEnum;

namespace boost
{
	namespace filesystem
	{
		class path;
	}
}

using Path = boost::filesystem::path;

#endif