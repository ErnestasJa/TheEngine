#include "Precomp.h"
#include "ShaderLoader.h"
#include "opengl/material/Shader.h"
#include "modules/logging/Logger.h"
#include "application/AppContext.h"
#include "modules/filesystem/FileSystem.h"

ShaderLoader::ShaderLoader()
{
}

ShaderLoader::~ShaderLoader()
{
	//dtor
}

ShaderPtr ShaderLoader::Load(const Path & vertex_file_name, const Path & fragment_file_name, bool replaceCached)
{
	Path resourceName = vertex_file_name.filename().generic_string() + fragment_file_name.filename().generic_string();

	Resource<Shader> existingResource = this->GetResource(resourceName);

	if (existingResource.resource && !replaceCached)
	{
		GetContext().GetLogger()->log(LOG_LOG, "Shader returned from cache.");
		return existingResource.resource;
	}

	FilePtr vertexFile = GetContext().GetFileSystem()->OpenRead(vertex_file_name);
	FilePtr fragmentFile = GetContext().GetFileSystem()->OpenRead(fragment_file_name);

	if (!vertexFile->IsOpen() || !fragmentFile->IsOpen())
	{
		return ShaderPtr();
	}

	GetContext().GetLogger()->log(LOG_LOG, "Shader resource name: %s", resourceName.generic_string().c_str());

	ByteBufferPtr vertexBuffer = vertexFile->ReadText();
	ByteBufferPtr fragmentBuffer = fragmentFile->ReadText();

	Shader * shader = new Shader(resourceName.generic_string(), (char*)vertexBuffer->data(), (char*)fragmentBuffer->data(), "");
	shader->Compile();

	if (shader->IsCompiledAndLinked())
	{
		if (existingResource.resource)
		{
			RemoveResource(existingResource.path);
			GetContext().GetLogger()->log(LOG_LOG, "Removed cached shader: '%s'.", resourceName.c_str());
		}

		GetContext().GetLogger()->log(LOG_LOG, "Shader loaded: '%s'.", resourceName.c_str());

		Resource<Shader> res(ShaderPtr(shader), resourceName);
		this->AddResource(res);
		return res.resource;
	}
	else
	{
		delete shader;

		if (existingResource.resource)
			GetContext().GetLogger()->log(LOG_ERROR, "Shader failed to load: '%s', using cached version.", resourceName.generic_string().c_str());
		else
			GetContext().GetLogger()->log(LOG_ERROR, "Shader failed to load: '%s'.", resourceName.generic_string().c_str());

		return existingResource.resource;
	}
}

ShaderPtr ShaderLoader::Load(const Path & fileName, bool replaceCached)
{
	Resource<Shader> existingResource = this->GetResource(fileName);

	if (existingResource.resource && replaceCached == false)
	{
		GetContext().GetLogger()->log(LOG_LOG, "Found shader in cache, skipping loading.");
		return existingResource.resource;
	}

	ByteBufferPtr vertexBuffer = nullptr, fragmentBuffer = nullptr, geometryBuffer = nullptr;

	FilePtr vertexFile = GetContext().GetFileSystem()->OpenRead(Path(fileName).replace_extension(".vert"));
	vertexBuffer = vertexFile->ReadText();

	FilePtr fragmentFile = GetContext().GetFileSystem()->OpenRead(Path(fileName).replace_extension(".frag"));
	fragmentBuffer = fragmentFile->ReadText();

	if (GetContext().GetFileSystem()->FileExists(Path(fileName).replace_extension(".geom")))
	{
		FilePtr geometryFile = GetContext().GetFileSystem()->OpenRead(Path(fileName).replace_extension(".geom"));

		if (geometryFile)
		{
			geometryBuffer = geometryFile->ReadText();
		}
	}

	Path resourceName = fileName.filename();
	Shader * sh = nullptr;

	if (!vertexBuffer && !fragmentBuffer)
		return existingResource.resource;

	if (geometryBuffer)
	{
		sh = new Shader(resourceName.generic_string(), (char*)vertexBuffer->data(), (char*)fragmentBuffer->data(), (char*)geometryBuffer->data());
	}
	else
	{
		sh = new Shader(resourceName.generic_string(), (char*)vertexBuffer->data(), (char*)fragmentBuffer->data());
	}

	sh->Compile();

	if (sh->IsCompiledAndLinked())
	{
		if (existingResource.resource)
		{
			RemoveResource(existingResource.path);
			GetContext().GetLogger()->log(LOG_LOG, "Removed cached shader: '%s'.", fileName.generic_string().c_str());
		}
		GetContext().GetLogger()->log(LOG_LOG, "Shader loaded: '%s'.", fileName.generic_string().c_str());

		Resource<Shader> res(ShaderPtr(sh), fileName);
		this->AddResource(res);
		return res.resource;
	}
	else
	{
		delete sh;

		if (existingResource.resource)
			GetContext().GetLogger()->log(LOG_ERROR, "Shader failed to load: '%s', using cached version.", resourceName.generic_string().c_str());
		else
			GetContext().GetLogger()->log(LOG_ERROR, "Shader failed to load: '%s'.", resourceName.generic_string().c_str());

		return existingResource.resource;
	}
}

ShaderPtr ShaderLoader::GetShaderByName(const Path & name)
{
	for (Resource<Shader> & res : m_resources)
	{
		if (res.resource->GetName() == name)
			return res.resource;
	}

	return ShaderPtr();
}