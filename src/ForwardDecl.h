#ifndef FORWARD_DECL_H
#define FORWARD_DECL_H

namespace std
{
	template <class T>	class shared_ptr;
	template <class T>	class weak_ptr;
}


#define DECLARE_PTR(var)\
	class var;\
	typedef std::shared_ptr<var> var##Ptr;\
	typedef std::weak_ptr<var> var##WeakPtr

DECLARE_PTR(AppContext);
DECLARE_PTR(ApplicationSettingsManager);
DECLARE_PTR(ApplicationWindow);
DECLARE_PTR(Camera);
DECLARE_PTR(FileSystem);
DECLARE_PTR(GUIEnvironment);
DECLARE_PTR(IBindingValue);
DECLARE_PTR(IFile);
DECLARE_PTR(Image);
DECLARE_PTR(ImageLoader);
DECLARE_PTR(InputHandler);
DECLARE_PTR(Logger);
DECLARE_PTR(Mesh);
DECLARE_PTR(MeshLoader);
DECLARE_PTR(OpenGLExtensionLoader);
DECLARE_PTR(ResourceManager);
DECLARE_PTR(Shader);
DECLARE_PTR(ShaderLoader);
DECLARE_PTR(Timer);
DECLARE_PTR(VarGroup);
DECLARE_PTR(Material);
DECLARE_PTR(Texture);
DECLARE_PTR(IRenderObject);
DECLARE_PTR(RenderStateManager);
DECLARE_PTR(EngineProvider);
DECLARE_PTR(IEngineModule);

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