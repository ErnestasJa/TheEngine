#pragma once

struct Binding
{
	std::string name;
	int32_t index;
	uint32_t Type;
};

/**
 * Usage:
 * shader sh(shader_name, vertex_source, fragment_source);
 * sh->compile();
 * if(sh->program) good;
 *
 * ///Render
 * sh->set();
 **/

#include "utility/Vector.h"

struct Shader
{
public:
	Shader(const std::string & name, const std::string & vsstr, const std::string & fsstr);
	Shader(const std::string & name, const std::string & vsstr, const std::string & fsstr, const std::string & gsstr);
	virtual ~Shader();

	bool Compile();
	bool Link();
	bool IsCompiledAndLinked();
	void Set();
	const vector<Binding> & GetBindings() const;
	Binding GetBinding(const std::string & pname) const;
	const std::string & GetName() const;
	void QueryAllBindingLocations();
	[[deprecated]] int32_t getparam(const std::string & pname);

private:
	static void ShowCompilationInfo(uint32_t obj, const std::string & tname, const std::string & name);
	bool CompileShaderObject(GLenum Type, uint32_t &obj, const std::string & def, const std::string & tname, const std::string & name);

private:
	std::string name, vsstr, fsstr, gsstr;
	uint32_t program, vsobj, fsobj, gsobj;
	vector<Binding> bindings;
};

typedef std::shared_ptr<Shader> ShaderPtr;
