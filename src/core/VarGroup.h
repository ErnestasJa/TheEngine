#ifndef VARGROUP_H
#define VARGROUP_H

#include "core/Vector.h"
#include <boost/core/noncopyable.hpp>

class Var;
class VarGroup: private boost::noncopyable
{
public:
	static VarGroup Null;

public:
	VarGroup();
	VarGroup(const char * groupName);
	VarGroup(const VarGroup & o);
	VarGroup & operator = (const VarGroup & other);
	virtual ~VarGroup();
	const char * Name() const;

	vector<Var> & Vars();
	const vector<Var> & Vars() const;
	vector<VarGroup> & Groups();
	const vector<VarGroup> & Groups() const;
	Var & GetVar(const char * name);
	VarGroup & AddGroup(const char * name);
	VarGroup & AddGroup(const VarGroup & vGroup);
	bool AddVar(const Var & variable);
	VarGroup & GetGroup(const char * name);

private:
	char * m_name;
	vector<Var> m_vars;
	vector<VarGroup> m_groups;
};

typedef std::shared_ptr<VarGroup> VarGroupPtr;

#endif
