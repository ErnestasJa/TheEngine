#ifndef RENDER_STATEMANAGER_H
#define RENDER_STATEMANAGER_H

#include "core/Vector.h"
#include "ForwardDecl.h"

class RenderStateManager
{
public:
	static RenderStateManager & Instance();
	virtual ~RenderStateManager();
	void Enable(GLEnum glCapability);
	void Enable(vector<GLEnum> glCapabilities);
	void Disable(GLEnum glCapability);
	void Disable(vector<GLEnum> glCapabilities);
	void SetCamera(CameraPtr camera);
	CameraPtr GetCamera();
	void SetClearColor(glm::vec4 color);
	void SetClearColor(float r, float g, float b, float a);
	void SetCullFace(GLEnum cullFace);
	void SetFrontFace(GLEnum direction);

private:
	RenderStateManager();
private:
	CameraPtr   m_activeCamera;
};

RenderStateManager & GetRenderStateManager();
#endif