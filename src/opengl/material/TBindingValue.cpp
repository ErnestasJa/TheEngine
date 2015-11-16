#include "Precomp.h"
#include "IBindingValue.h"
#include "TBindingValue.h"

template <>
void TBindingValue<int32_t>::Set(int32_t index) 
{
	glUniform1i(index, m_value);
}

template <>
void TBindingValue<float>::Set(int32_t index) 
{
	glUniform1f(index, m_value);
}

template <>
void TBindingValue<glm::mat4x4>::Set(int32_t index) 
{
	glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(m_value));
}

template <>
void TBindingValue<glm::mat3x3>::Set(int32_t index) 
{
	glUniformMatrix3fv(index, 1, GL_FALSE, glm::value_ptr(m_value));
}

template <>
void TBindingValue<glm::vec2>::Set(int32_t index)
{
	glUniform2fv(index, 1, glm::value_ptr(m_value));
}

template <>
void TBindingValue<glm::vec3>::Set(int32_t index) 
{
	glUniform3fv(index, 1, glm::value_ptr(m_value));
}

template <>
void TBindingValue<glm::vec4>::Set(int32_t index) 
{
	glUniform4fv(index, 1, glm::value_ptr(m_value));
}