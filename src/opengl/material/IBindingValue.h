#ifndef IBINDING_VALUE_H
#define IBINDING_VALUE_H

class IBindingValue
{
public:
	virtual void Set(int32_t index) = 0;
};

typedef std::shared_ptr<IBindingValue> IBindingValuePtr;

#endif