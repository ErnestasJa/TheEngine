#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include "opengl/Shader.h"
#include "ResourceCache.h"
#include "ForwardDecl.h"

class AppContext;
class shader_loader : public resource_cache<Shader>
{
public:
	shader_loader();
	virtual ~shader_loader();

	ShaderPtr load(const Path & file);
	ShaderPtr load(const Path & vertex_file, const Path & fragment_file);
	ShaderPtr get_shader_by_name(const Path & name);

protected:
};

#endif // SHADER_LOADER_H
