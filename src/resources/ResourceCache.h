#pragma once
#include "utility/Vector.h"
#include "ForwardDecl.h"
#include "boost/filesystem/path.hpp"

template <class T>
struct resource
{
	Path _path;
	std::shared_ptr<T> _resource;
};

template <class T>
class resource_cache
{
public:

	void add_resource(resource<T> res);
	resource<T> get_resource(const Path & path);

protected:
	vector< resource<T> >  m_resources;
};

template <class T>
vector<std::shared_ptr<T> > * create_resource_cache()
{
	return new vector<std::shared_ptr<T> >();
}
