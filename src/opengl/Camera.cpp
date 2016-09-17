#include "Precomp.h"
#include "utility/Logger.h"
#include "Camera.h"
#include "application/AppContext.h"
#include "opengl/AABB.h"
#include "application/Window.h"

Camera::Camera(const glm::vec3 &pos, const glm::vec3 &target, const glm::vec3 &up, float aspect_ratio, float field_of_view, float near_z, float far_z)
{
	m_fps = true;
	m_hasFrustum = false;
	m_pos = pos;
	m_target = target;
	m_rot = glm::toQuat(glm::inverse(glm::lookAt(pos, target, up)));

	this->m_look = pos - target;
	this->m_up = up;

	m_fov = field_of_view;
	m_far = far_z;
	m_near = near_z;
	m_aspect_ratio = aspect_ratio;

	float tang = (float)glm::tan(glm::radians(m_fov)*0.5);

	nearHeight = m_near*tang;
	nearWidth = nearHeight*m_aspect_ratio;
	farHeight = m_far*tang;
	farWidth = farHeight*m_aspect_ratio;

	m_P = glm::perspective(field_of_view, aspect_ratio, near_z, far_z);
}

Camera::~Camera()
{
	GetContext().GetWindow()->SetCursorDisabled(false);
}

void Camera::SetFPS(bool fps)
{
	m_fps = fps;
	GetContext().GetWindow()->SetCursorDisabled(fps);
}

void Camera::ResetOrientation(glm::vec3 lookDir)
{
	//m_rot = glm::toQuat(glm::inverse(glm::lookAt(m_pos, m_pos + lookDir * 10.f, m_up)));
	m_P = glm::perspective(m_fov, m_aspect_ratio, m_near, m_far);
}

void Camera::InitFrustum()
{
	m_hasFrustum = true;

	auto proj = GetViewProjMat();
	frustumPlanes[FP_RIGHT].SetNormalsAndD(
		proj[0][3] - proj[0][0],
		proj[1][3] - proj[1][0],
		proj[2][3] - proj[2][0],
		proj[3][3] - proj[3][0]);

	frustumPlanes[FP_LEFT].SetNormalsAndD(
		proj[0][3] + proj[0][0],
		proj[1][3] + proj[1][0],
		proj[2][3] + proj[2][0],
		proj[3][3] + proj[3][0]);

	frustumPlanes[FP_BOTTOM].SetNormalsAndD(
		proj[0][3] + proj[0][1],
		proj[1][3] + proj[1][1],
		proj[2][3] + proj[2][1],
		proj[3][3] + proj[3][1]);

	frustumPlanes[FP_TOP].SetNormalsAndD(
		proj[0][3] - proj[0][1],
		proj[1][3] - proj[1][1],
		proj[2][3] - proj[2][1],
		proj[3][3] - proj[3][1]);

	frustumPlanes[FP_FAR].SetNormalsAndD(
		proj[0][3] - proj[0][2],
		proj[1][3] - proj[1][2],
		proj[2][3] - proj[2][2],
		proj[3][3] - proj[3][2]);

	frustumPlanes[FP_NEAR].SetNormalsAndD(
		proj[0][3] + proj[0][2],
		proj[1][3] + proj[1][2],
		proj[2][3] + proj[2][2],
		proj[3][3] + proj[3][2]);
}

//void Camera::InitFrustum()
//{
//	glm::vec3 Z = glm::normalize(-m_look);
//
//	glm::vec3 X = glm::normalize(glm::cross(m_up, Z));
//
//	glm::vec3 Y = glm::cross(Z, X);
//
//	glm::vec3 nearCenter = m_pos - Z * m_near;
//	glm::vec3 farCenter = m_pos - Z * m_far;
//
//	// compute the 4 corners of the frustum on the near plane
//	nearTopLeft = nearCenter + Y * nearHeight - X * nearWidth;
//	nearTopRight = nearCenter + Y * nearHeight + X * nearWidth;
//	nearBottomLeft = nearCenter - Y * nearHeight - X * nearWidth;
//	nearBottomRight = nearCenter - Y * nearHeight + X * nearWidth;
//
//	// compute the 4 corners of the frustum on the far plane
//	farTopLeft = farCenter + Y * farHeight - X * farWidth;
//	farTopRight = farCenter + Y * farHeight + X * farWidth;
//	farBottomLeft = farCenter - Y * farHeight - X * farWidth;
//	farBottomRight = farCenter - Y * farHeight + X * farWidth;
//
//	// compute the six planes
//	// the function set3Points assumes that the points
//	// are given in counter clockwise order
//	printf("FP_TOP");
//	frustumPlanes[FP_TOP].SetPoints(nearTopRight, nearTopLeft, farTopLeft, farTopRight); // top plane
//	printf("FP_BOTTOM");
//	frustumPlanes[FP_BOTTOM].SetPoints(nearBottomLeft, nearBottomRight, farBottomRight, farBottomLeft); // bottom plane
//	printf("FP_LEFT");
//	frustumPlanes[FP_LEFT].SetPoints(nearTopLeft, nearBottomLeft, farBottomLeft, farTopLeft); // left plane
//	printf("FP_RIGHT");
//	frustumPlanes[FP_RIGHT].SetPoints(nearBottomRight, nearTopRight, farTopRight, farBottomRight); // right plane
//	printf("FP_FAR");
//	frustumPlanes[FP_FAR].SetPoints(farTopRight, farTopLeft, farBottomLeft, farBottomRight); // far plane
//	printf("FP_NEAR");
//	frustumPlanes[FP_NEAR].SetPoints(nearTopLeft, nearTopRight, nearBottomRight, nearBottomLeft); // near plane
//}

Plane3d * Camera::GetFrustumPlanes()
{
	return frustumPlanes;
}

INTERSECT_RESULT Camera::PointInFrustum(const glm::vec3 &point)
{
	INTERSECT_RESULT res = IR_INSIDE;
	int out = 0;
	loop(i, 6)
	{
		if (frustumPlanes[i].Distance(point) <= 0)
			out++;
	}
	if (out > 0)
	{
		res = IR_OUTSIDE;
	}
	return res;
}

INTERSECT_RESULT Camera::SphereInFrustum(const glm::vec3 &center, float radius)
{
	INTERSECT_RESULT res = IR_INSIDE;

	int out = 0;
	loop(i, 6)
	{
		if (frustumPlanes[i].Distance(center, radius) <= 0)
			return IR_OUTSIDE;
	}

	return res;
}

INTERSECT_RESULT Camera::BoxInFrustum(const AABB &box)
{
	auto points = box.CalculatePoints();

	INTERSECT_RESULT res = IR_INSIDE;

	int in = 0;

	for (auto i = 0; i < 6; i++)
	{
		in = 0;

		for (int j = 0; j < 8 && in == 0; j++)
		{
			if (frustumPlanes[i].Distance(points[j]) <= 0)
				return IR_OUTSIDE;
			else
			{
				in++;
				res = IR_INTERSECT;
			}
		}
	}

	return res;
}

glm::mat4 & Camera::GetProjectionMat()
{
	return m_P;
}

const float Camera::GetFOV() const
{
	return m_fov;
}

const float Camera::GetFar() const
{
	return m_far;
}

const float Camera::GetNear() const
{
	return m_near;
}

const float Camera::GetAspectRatio() const
{
	return m_aspect_ratio;
}

const glm::vec3 Camera::GetLook() const
{
	return m_look;
}

const glm::vec3 Camera::GetRight() const
{
	return m_right;
}

const glm::vec3 Camera::GetPosition() const
{
	return m_pos;
}

void Camera::SetPosition(glm::vec3 pos)
{
	m_pos = pos;
}

const glm::vec3 &Camera::GetTarget() const
{
	return m_target;
}

void Camera::SetTarget(glm::vec3 target)
{
	m_target = target;
}

void Camera::SetRotation(const glm::quat &rotation)
{
	m_rot = rotation;
}

void Camera::SetProjection(const glm::mat4 &projection)
{
	m_P = projection;
}

void Camera::Update(float dt)
{
	m_pos += m_translation;
	m_translation = glm::vec3(0);

	if (m_fps)
	{
		auto window = GetContext().GetWindow();
		glm::ivec2 s = window->GetWindowSize();
		m_current_mouse_pos = window->GetMousePos();
		window->SetMousePos(s / 2);
		m_last_mouse_pos = window->GetMousePos();
		HandleMouse();
	}
	else
	{
		//m_rot = glm::toQuat(glm::inverse(glm::lookAt(m_pos, m_target, m_up)));
		m_look = glm::vec3(m_rot*glm::vec3(0, 0, -1));
		m_up = glm::vec3(m_rot*glm::vec3(0, 1, 0));

		m_right = glm::cross(m_look, m_up);
	}

	InitFrustum();
}

void Camera::Walk(const float amount)
{
	m_translation += (m_look*amount);
}

void Camera::Strafe(const float amount)
{
	m_translation += (m_right*amount);
}

void Camera::Lift(const float amount)
{
	m_translation += (m_up*amount);
}

glm::mat4x4 Camera::GetViewMat()
{
	m_look = glm::vec3(m_rot*glm::vec3(0, 0, -1));
	m_up = glm::vec3(m_rot*glm::vec3(0, 1, 0));

	m_right = glm::cross(m_look, m_up);

	m_target = m_pos + m_look;

	return glm::lookAt(m_pos, m_target, m_up);
}

glm::mat4x4 Camera::GetViewProjMat()
{
	return GetProjectionMat()*GetViewMat();
}

void Camera::Orbit(glm::vec3 point, float distance, float verticalAngle, float horizontalAngle)
{
	float camX = distance * sin(glm::radians(horizontalAngle)) * sin(glm::radians(verticalAngle));
	float camY = distance * cos(glm::radians(verticalAngle));
	float camZ = distance * cos(glm::radians(horizontalAngle)) * sin(glm::radians(verticalAngle));

	m_pos = point + glm::vec3(camX, camY, camZ);
	m_rot = glm::toQuat(glm::inverse(glm::lookAt(m_pos, point, glm::vec3(0, 1, 0))));
	m_target = point;
	m_look = m_target - m_pos;
}

void Camera::HandleMouse()
{
	glm::ivec2 delta_pos = m_current_mouse_pos - m_last_mouse_pos;

	if (glm::abs(delta_pos.x) > 64 || glm::abs(delta_pos.y) > 64) ///probably some random shit happened, discard
		return;

	glm::quat r = m_rot;

	glm::quat rot_x(glm::vec3(0, -delta_pos.x / 100.0f, 0)), rot_y(glm::vec3(-delta_pos.y / 100.0f, 0, 0));

	r = rot_x * r;
	r = r * rot_y;

	m_rot = r;
}