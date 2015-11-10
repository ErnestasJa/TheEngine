#include "Precomp.h"
#include "RenderStateManager.h"
#include "application/AppContext.h"
#include "utility/Logger.h"


RenderStateManager & GetRenderStateManager()
{
	return RenderStateManager::Instance();
}

RenderStateManager::RenderStateManager()
{
	
}

RenderStateManager::~RenderStateManager()
{

}

void RenderStateManager::Enable(GLEnum glCapability)
{
	glEnable(glCapability);
}

void RenderStateManager::Enable(vector<GLEnum> glCapabilities)
{
	for (GLEnum glCapability : glCapabilities)
		glEnable(glCapability);
}

void RenderStateManager::Disable(GLEnum glCapability)
{
	glDisable(glCapability);
}

void RenderStateManager::Disable(vector<GLEnum> glCapabilities)
{
	for (GLEnum glCapability : glCapabilities)
		glDisable(glCapability);
}

void RenderStateManager::SetCamera(CameraPtr camera)
{
	m_activeCamera = camera;
}

CameraPtr RenderStateManager::GetCamera()
{
	return m_activeCamera;
}

void RenderStateManager::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void RenderStateManager::SetClearColor(glm::vec4 color)
{
	glClearColor(color.x, color.y, color.z, color.w);
}

void RenderStateManager::SetCullFace(GLEnum cullFace)
{
	glCullFace(cullFace);
}

void RenderStateManager::SetFrontFace(GLEnum frontFace)
{
	glFrontFace(frontFace);
}
