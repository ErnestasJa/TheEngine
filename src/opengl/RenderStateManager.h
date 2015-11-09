#ifndef RENDER_STATEMANAGER_H
#define RENDER_STATEMANAGER_H

#include "utility/Vector.h"
#include "ForwardDecl.h"

class RenderStateManager
{
public:
	RenderStateManager(MaterialPtr defaultMaterial);
	virtual ~RenderStateManager();

	void Enable(GLEnum glCapability);
	void Enable(vector<GLEnum> glCapabilities);
	void Disable(GLEnum glCapability);
	void Disable(vector<GLEnum> glCapabilities);
	void SetMaterial(MaterialPtr material);
	MaterialPtr GetMaterial();
	MaterialPtr GetDefaultMaterial();
	void SetCamera(CameraPtr camera);
	CameraPtr GetCamera();
	void SetClearColor(glm::vec4 color);
	void SetClearColor(float r, float g, float b, float a);
	void SetCullFace(GLEnum cullFace);
	void SetFrontFace(GLEnum direction);

private:
	MaterialPtr m_activeMaterial;
	MaterialPtr m_defaultMaterial;
	CameraPtr   m_activeCamera;
};

#endif