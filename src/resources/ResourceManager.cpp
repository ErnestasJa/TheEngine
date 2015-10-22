#include "Precomp.h"
#include "ImageLoader.h"
#include "ShaderLoader.h"
#include "MeshLoader.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	m_shaderLoader = new ShaderLoader();
	m_imageLoader = new ImageLoader();
	m_meshLoader = new MeshLoader();
}

ResourceManager::~ResourceManager()
{
	delete m_shaderLoader;
	delete m_meshLoader;
	delete m_imageLoader;
}


ImagePtr ResourceManager::LoadImage(const Path & filePath)
{
	return GetImageLoader()->Load(filePath);
}

MeshPtr ResourceManager::LoadMesh(const Path & filePath)
{
	return GetMeshLoader()->Load(filePath);
}

ShaderPtr ResourceManager::LoadShader(const Path & shaderPath)
{
	return GetShaderLoader()->Load(shaderPath);
}

ShaderPtr ResourceManager::LoadShader(const Path & vertexPath, const Path & fragmentPath)
{
	return GetShaderLoader()->Load(vertexPath, fragmentPath);
}

ImageLoader * ResourceManager::GetImageLoader()
{
	return m_imageLoader;
}

ShaderLoader * ResourceManager::GetShaderLoader()
{
	return m_shaderLoader;
}

MeshLoader * ResourceManager::GetMeshLoader()
{
	return m_meshLoader;
}