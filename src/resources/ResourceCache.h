#pragma once
#include "utility/Vector.h"
#include "ForwardDecl.h"
#include "boost/filesystem/path.hpp"

template <class T>
struct Resource
{
	Path path;
	std::shared_ptr<T> resource;

	Resource()
	{

	}

	Resource(std::shared_ptr<T> resourcePtr, Path resourcePath)
	{
		this->path = resourcePath;
		this->resource = resourcePtr;
	}
};

template <class T>
class ResourceCache
{
public:

	void AddResource(Resource<T> res)
	{
		m_resources.push_back(res);
	}

	Resource<T> GetResource(const Path & path)
	{
		auto it = std::find_if(m_resources.begin(), m_resources.end(), [&path](Resource<T> res)
		{
			return res.path == path;
		});

		if (it != m_resources.end())
		{
			return (*it);
		}

		return Resource<T>();
	}

	bool RemoveResource(const Path & path)
	{
		auto it = std::find_if(m_resources.begin(), m_resources.end(), [&path](Resource<T> res)
		{
			return res.path == path;
		});

		if (it != m_resources.end())
		{
			m_resources.erase(it);
			return true;
		}

		return false;
	}


	void Clear()
	{
		m_resources.clear();
	}

protected:
	vector< Resource<T> >  m_resources;
};

template <class T>
vector<std::shared_ptr<T> > * create_resource_cache()
{
	return new vector<std::shared_ptr<T> >();
}
