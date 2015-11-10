#ifndef IRENDER_OBJECT_H
#define IRENDER_OBJECT_h

class IRenderObject
{
public:
	IRenderObject();
	virtual ~IRenderObject();
	virtual void Render() = 0;
};

typedef std::shared_ptr<IRenderObject> IRenderObjectPtr;

#endif