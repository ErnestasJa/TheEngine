#ifndef SHADER_BINDING_H
#define SHADER_BINDING_H

#include "ForwardDecl.h"

class ShaderBinding
{
public:
	ShaderBinding();
	ShaderBinding(int32_t index, const std::string & name, GLEnum openGLType);
	ShaderBinding(ShaderBinding && other);
	ShaderBinding(const ShaderBinding & other) = delete;
	ShaderBinding& operator=(ShaderBinding && other);
	~ShaderBinding();


	void Set(float value);
	void Set(int32_t value);
	void Set(glm::mat4x4 value);
	void Set(glm::mat3x3 value);
	void Set(glm::vec3 value);
	void Set(glm::vec4 value);

	void Update();
	bool HasValue();

	int32_t GetIndex();
	const std::string & GetName();
	uint32_t GetOpenGLType();

	IBindingValuePtr GetBindingValue();

private:
	int32_t m_bindingIndex;
	std::string m_name;
	GLEnum m_openGLType;
	IBindingValuePtr m_value;
};

#endif