#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include "ResourceCache.h"
#include "ForwardDecl.h"
#include "boost/filesystem/path.hpp"

class AppContext;
class ShaderLoader : public ResourceCache<Shader>
{
public:
	ShaderLoader();
	virtual ~ShaderLoader();

	ShaderPtr Load(const Path & file, bool replaceCached = false);
	ShaderPtr Load(const Path & vertex_file, const Path & fragment_file, bool replaceCached = false);
	ShaderPtr GetShaderByName(const Path & name);

protected:
};

#endif // SHADER_LOADER_H
