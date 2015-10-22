#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "ForwardDecl.h"

class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();

	ImagePtr LoadImage(const Path & filePath);
	MeshPtr LoadMesh(const Path & filePath);
	ShaderPtr LoadShader(const Path & shaderPath);
	ShaderPtr LoadShader(const Path & vertexPath, const Path & fragmentPath);

protected: 
	ImageLoader * GetImageLoader();
	ShaderLoader * GetShaderLoader();
	MeshLoader * GetMeshLoader();
private:
	ImageLoader * m_imageLoader;
	ShaderLoader * m_shaderLoader;
	MeshLoader * m_meshLoader;
};

#endif