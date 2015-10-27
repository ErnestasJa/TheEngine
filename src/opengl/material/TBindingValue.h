#ifndef TBINDING_VALUE
#define TBINDING_VALUE

#include "ForwardDecl.h"

template <class T>
class TBindingValue: public IBindingValue
{
public:
	TBindingValue(T value);
	virtual ~TBindingValue();
	virtual void Set(int32_t index);

private:
	T m_value;
};

template <> void TBindingValue<float>::Set(int32_t index);
template <> void TBindingValue<int32_t>::Set(int32_t index);
template <> void TBindingValue<glm::mat4x4>::Set(int32_t index);
template <> void TBindingValue<glm::mat3x3>::Set(int32_t index);
template <> void TBindingValue<glm::vec3>::Set(int32_t index);
template <> void TBindingValue<glm::vec4>::Set(int32_t index);

template <class T>
TBindingValue<T>::TBindingValue(T value)
{
	m_value = value;
}

template <class T>
TBindingValue<T>::~TBindingValue()
{

}

#endif