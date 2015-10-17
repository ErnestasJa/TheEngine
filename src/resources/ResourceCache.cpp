#include "Precomp.h"
#include "ResourceCache.h"

template <class T>
void resource_cache<T>::add_resource(resource<T> res)
{
	m_resources.push_back(res);
}

template <class T>
resource<T> resource_cache<T>::get_resource(const Path & path)
{
	auto it = std::find_if(m_resources.begin(), m_resources.end(), [&path](resource<T> res)
	{
		return res._path == path;
	});

	if (it != m_resources.end())
	{
		return (*it);
	}

	return resource<T>();
}
