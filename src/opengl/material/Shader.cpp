#include "Precomp.h"
#include "Shader.h"
#include "application/AppContext.h"
#include "utility/Logger.h"


uint32_t Shader::currentProgram = -1;

void Shader::ShowCompilationInfo(uint32_t obj, const std::string & tname, const std::string & name)
{
	int32_t length = 0;

	if (tname == "PROG")
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
	else
		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);

	if (length > 1)
	{
		GLchar *log = new GLchar[length];

		if (tname == "PROG")
			glGetProgramInfoLog(obj, length, &length, log);
		else
			glGetShaderInfoLog(obj, length, &length, log);

		GetContext().GetLogger()->log(LOG_ERROR, "GLSL ERROR (%s:%s): [%i] %s\n", tname.c_str(), name.c_str(), length, log);

		delete[] log;
	}
}

bool Shader::CompileShaderObject(GLenum Type, uint32_t &obj, const std::string & def, const std::string & tname, const std::string & name)
{
	obj = glCreateShader(Type);
	char const * str = def.c_str();

	glShaderSource(obj, 1, &str, nullptr);
	glCompileShader(obj);

	int32_t success;
	glGetShaderiv(obj, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		Shader::ShowCompilationInfo(obj, tname, name);
		glDeleteShader(obj);
		obj = 0;
	}

	return obj != 0;
}

Shader::Shader(const std::string & name, const std::string & vsstr, const std::string & fsstr)
	: name(name), vsstr(vsstr), fsstr(fsstr), program(0), vsobj(0), fsobj(0)
{
}

Shader::Shader(const std::string & name, const std::string & vsstr, const std::string & fsstr, const std::string & gsstr)
	: name(name), vsstr(vsstr), fsstr(fsstr), gsstr(gsstr), program(0), vsobj(0), fsobj(0), gsobj(0)
{
};

Shader::~Shader()
{
	if (program)
		glDeleteProgram(program);
}

bool Shader::Compile()
{
	bool vertexCompileSuccess = CompileShaderObject(GL_VERTEX_SHADER, vsobj, vsstr, "VS", name);
	bool fragmentSuccess = CompileShaderObject(GL_FRAGMENT_SHADER, fsobj, fsstr, "PS", name);
	bool geometrySuccess = true;
	if(gsstr.size() > 0) geometrySuccess = CompileShaderObject(GL_GEOMETRY_SHADER, gsobj, gsstr, "GS", name);

	if(!vertexCompileSuccess && !fragmentSuccess && !geometrySuccess)
		return false;

	return Link();
}

bool Shader::Link()
{
	program = vsobj && fsobj ? glCreateProgram() : 0;
	int32_t success = 0;

	if (program)
	{
		glAttachShader(program, vsobj);
		glAttachShader(program, fsobj);
		if (gsobj) glAttachShader(program, gsobj);

		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &success);

		if (!success)
		{
			if (program)
			{
				ShowCompilationInfo(program, "PROG", name);
				glDeleteProgram(program);
				program = 0;
			}

			GetContext().GetLogger()->log(LOG_ERROR, "Failed to link shader '%s'.", name);
		}
		else
		{
			glDetachShader(program, vsobj);
			glDetachShader(program, fsobj);
			if (gsobj) glDetachShader(program, gsobj);
			QueryAllBindingLocations();
		}
	}

	return program != 0;
}

bool Shader::IsCompiledAndLinked()
{
	return program != 0;
}

void Shader::Set()
{
	if(program!=Shader::currentProgram)
	{
		GetContext().GetLogger()->log(loglevel::LOG_LOG, "stuff is set");
		glUseProgram(program);
		Shader::currentProgram = program;
	}

	for (ShaderBinding &t : m_bindings)
	{
		if (t.HasValue())
			t.Update();
	}
}

int32_t Shader::getparam(const std::string & pname)
{
	return  glGetUniformLocation(program, pname.c_str());
}

bool Shader::HasBinding(const std::string & name)
{
	for (ShaderBinding &t : m_bindings)
	{
		if (t.GetName() == name)
			return true;
	}

	return false;
}

ShaderBinding & Shader::GetBinding(const std::string & pname)
{
	for (ShaderBinding &t : m_bindings)
	{
		if (t.GetName() == pname)
			return t;
	}

	return m_nullBinding;
}

const std::string & Shader::GetName() const
{
	return name;
}

void Shader::QueryAllBindingLocations()
{
	m_bindings.clear();

	int32_t total = -1;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &total);

	GetContext().GetLogger()->log(LOG_LOG, "Binding count: %i\n", total);

	for (int i = 0; i < total; ++i)
	{
		int name_len = -1, num = -1;
		GLenum type = GL_ZERO;
		char name[100];
		glGetActiveUniform(program, GLuint(i), sizeof(name) - 1, &name_len, &num, &type, name);
		GLint location = glGetUniformLocation(program, name);

		m_bindings.push_back(ShaderBinding(location, name, type));
		GetContext().GetLogger()->log(LOG_LOG, "Binding index=%i; Name='%s'; Type=%i;\n", location, name, type);
	}
}