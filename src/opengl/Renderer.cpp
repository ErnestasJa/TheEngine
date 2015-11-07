#include "Precomp.h"
#include "Renderer.h"
#include "application/AppContext.h"
#include "utility/Logger.h"

Renderer::Renderer(MaterialPtr defaultMaterial)
{
	m_defaultMaterial = defaultMaterial;

	if (!m_defaultMaterial)
		GetContext().GetLogger()->log(LOG_WARN, "Default material is null.");
}

Renderer::~Renderer()
{

}

void Renderer::Enable(GLEnum glCapability)
{
	glEnable(glCapability);
}

void Renderer::Enable(vector<GLEnum> glCapabilities)
{
	for (GLEnum glCapability : glCapabilities)
		glEnable(glCapability);
}

void Renderer::Disable(GLEnum glCapability)
{
	glDisable(glCapability);
}

void Renderer::Disable(vector<GLEnum> glCapabilities)
{
	for (GLEnum glCapability : glCapabilities)
		glDisable(glCapability);
}

void Renderer::SetMaterial(MaterialPtr material)
{
	m_activeMaterial = material;
}

MaterialPtr Renderer::GetMaterial()
{
	return m_activeMaterial;
}

MaterialPtr Renderer::GetDefaultMaterial()
{
	return m_defaultMaterial;
}

void Renderer::SetCamera(CameraPtr camera)
{
	m_activeCamera = camera;
}

CameraPtr Renderer::GetCamera()
{
	return m_activeCamera;
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void Renderer::SetClearColor(glm::vec4 color)
{
	glClearColor(color.x, color.y, color.z, color.w);
}

void Renderer::SetCullFace(GLEnum cullFace)
{
	glCullFace(cullFace);
}

void Renderer::SetFrontFace(GLEnum frontFace)
{
	glFrontFace(frontFace);
}
