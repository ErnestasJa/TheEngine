#pragma once

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
#include "ForwardDecl.h"
#include "ShaderBinding.h"

class Shader
{
private:
	static uint32_t currentProgram;
public:
	Shader(const std::string & name, const std::string & vsstr, const std::string & fsstr);
	Shader(const std::string & name, const std::string & vsstr, const std::string & fsstr, const std::string & gsstr);
	virtual ~Shader();

	bool Compile();
	bool Link();
	bool IsCompiledAndLinked();
	void Set();
	const std::string & GetName() const;

	const vector<ShaderBinding> & GetBindings() const;
	ShaderBinding & GetBinding(const std::string & name);
	bool HasBinding(const std::string & name);
	const uint32_t GetProgramId() const;


private:
	void QueryAllBindingLocations();
	static void ShowCompilationInfo(uint32_t obj, const std::string & tname, const std::string & name);
	bool CompileShaderObject(GLenum Type, uint32_t &obj, const std::string & def, const std::string & tname, const std::string & name);

private:
	std::string name, vsstr, fsstr, gsstr;
	uint32_t program, vsobj, fsobj, gsobj;
	vector<ShaderBinding> m_bindings;
	ShaderBinding m_nullBinding;
};


typedef std::shared_ptr<Shader> ShaderPtr;
