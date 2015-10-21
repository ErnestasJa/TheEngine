#include "Precomp.h"
#include "ShaderLoader.h"
#include "utility/Logger.h"
#include "application/AppContext.h"
#include "core/FileSystem.h"
#include "boost/filesystem/path.hpp"

shader_loader::shader_loader()
{
}

shader_loader::~shader_loader()
{
	//dtor
}

ShaderPtr shader_loader::load(const Path & vertex_file_name, const Path & fragment_file_name)
{
	Path resourceName = vertex_file_name.filename().generic_string() + fragment_file_name.filename().generic_string();
	
	resource<Shader> res = this->get_resource(resourceName);

	if (res._resource)
	{
		GetContext().GetLogger()->log(LOG_LOG, "Shader returned from cache.");
		return res._resource;
	}

	FilePtr vertexFile = GetContext().GetFileSystem()->OpenRead(vertex_file_name);
	FilePtr fragmentFile = GetContext().GetFileSystem()->OpenRead(fragment_file_name);

	if(!vertexFile->IsOpen() || !fragmentFile->IsOpen())
	{
		return ShaderPtr();
	}

	GetContext().GetLogger()->log(LOG_LOG, "Shader resource name: %s", resourceName.generic_string().c_str());

	ByteBufferPtr vertexBuffer = vertexFile->ReadText();
	ByteBufferPtr fragmentBuffer = fragmentFile->ReadText();

	Shader * shader = new Shader(resourceName.generic_string(), (char*)vertexBuffer->data(), (char*)fragmentBuffer->data(), "");
	shader->Compile();
	shader->link();

	if (shader->program)
	{
		res._resource = ShaderPtr(shader);
		res._path = resourceName;
		this->add_resource(res);
	}

	if (res._resource)
		GetContext().GetLogger()->log(LOG_LOG, "Shader '%s' loaded.", resourceName.c_str());

	return res._resource;
}

ShaderPtr shader_loader::load(const Path & fileName)
{
	resource<Shader> res;

	res = this->get_resource(fileName);

	if (res._resource)
	{
		GetContext().GetLogger()->log(LOG_LOG, "Found shader in cache, skipping loading.");
		return res._resource;
	}

	FilePtr vertexFile = GetContext().GetFileSystem()->OpenRead(Path(fileName).replace_extension(".vert"));
	ByteBufferPtr vertexBuffer = vertexFile->ReadText();

	FilePtr fragmentFile = GetContext().GetFileSystem()->OpenRead(Path(fileName).replace_extension(".frag"));
	ByteBufferPtr fragmentBuffer = fragmentFile->ReadText();

	FilePtr geometryFile = GetContext().GetFileSystem()->OpenRead(Path(fileName).replace_extension(".geom"));
	ByteBufferPtr geometryBuffer = geometryFile->ReadText();

#if 0
	if(vertexBuffer)
	{
		GetContext().GetLogger()->log(LOG_LOG, "Vertex shader: %s", (char*)vertexBuffer->data());
	}
	
	if(fragmentBuffer)
	{
		GetContext().GetLogger()->log(LOG_LOG, "Fragment shader: %s", (char*)fragmentBuffer->data());
	}

	if (geometryBuffer)
	{
		GetContext().GetLogger()->log(LOG_LOG, "Geometry shader: %s", (char*)geometryBuffer->data());
	}
#endif

	Path resourceName = fileName.filename();
	GetContext().GetLogger()->log(LOG_LOG, "Shader name: %s", resourceName.generic_string().c_str());

	Shader * sh = nullptr;

	if(!vertexBuffer && !fragmentBuffer)
		return res._resource;

	if (geometryBuffer)
	{
		sh = new Shader(resourceName.generic_string(), (char*)vertexBuffer->data(), (char*)fragmentBuffer->data(), (char*)geometryBuffer->data());
	}
	else
	{
		sh = new Shader(resourceName.generic_string(), (char*)vertexBuffer->data(), (char*)fragmentBuffer->data());
	}

	sh->Compile();

	if (sh->program)
	{
		res._resource = ShaderPtr(sh);
		res._path = fileName;
		this->add_resource(res);
	}


	if (res._resource)
		GetContext().GetLogger()->log(LOG_LOG, "Shader '%s' loaded.", fileName.generic_string().c_str());

	return res._resource;
}

ShaderPtr shader_loader::get_shader_by_name(const Path & name)
{
	for (resource<Shader> & res : m_resources)
	{
		if (res._resource->name == name)
			return res._resource;
	}

	return ShaderPtr();
}