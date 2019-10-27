#ifndef CUBEMESH_H
#define CUBEMESH_H

#include "opengl/Mesh.h"
#include "opengl/BufferObject.h"
#include "resources/ResourceCache.h"

class CubeMesh
{
private:
	float m_size;
public:
	BufferObject<glm::vec3> * pos;
	BufferObject<glm::tvec4<uint8_t>> * col;
	BufferObject<glm::vec2> * tex_coords;
	BufferObject<glm::vec3> * normals;
	IndexBufferObject<uint32_t> * indices;
	glm::tvec4<uint8_t> m_color;

	std::shared_ptr<Mesh> glmesh;

	CubeMesh(float size = 1.0f, glm::tvec4<uint8_t> color = glm::tvec4<uint8_t>(1))
	{
		this->m_size = size;
		this->m_color = color;
		Init();
	}

	CubeMesh(const AABB & aabb, glm::tvec4<uint8_t> color = glm::tvec4<uint8_t>(1))
	{
		this->m_size = 1;
		this->m_color = color;
		Init(aabb);
	}

	~CubeMesh() {};

	bool Init()
	{
		glmesh = share(new Mesh());

		pos = new BufferObject<glm::vec3>(IBufferObject::USAGE_HINT::STATIC);

		pos->data = {
			{ m_size / 2.f, -m_size / 2.f, -m_size / 2.f },
			{ -m_size / 2.f, m_size / 2.f, -m_size / 2.f },
			{ m_size / 2.f, m_size / 2.f, -m_size / 2.f },
			{ -m_size / 2.f, m_size / 2.f, m_size / 2.f },
			{ m_size / 2.f, -m_size / 2.f, m_size / 2.f },
			{ m_size / 2.f, m_size / 2.f, m_size / 2.f },
			{ m_size / 2.f, m_size / 2.f, m_size / 2.f },
			{ m_size / 2.f, -m_size / 2.f, -m_size / 2.f },
			{ m_size / 2.f, m_size / 2.f, -m_size / 2.f },
			{ m_size / 2.f, -m_size / 2.f, m_size / 2.f },
			{ -m_size / 2.f, -m_size / 2.f, -m_size / 2.f },
			{ m_size / 2.f, -m_size / 2.f, -m_size / 2.f },
			{ -m_size / 2.f, -m_size / 2.f, -m_size / 2.f },
			{ -m_size / 2.f, m_size / 2.f, m_size / 2.f },
			{ -m_size / 2.f, m_size / 2.f, -m_size / 2.f },
			{ m_size / 2.f, m_size / 2.f, -m_size / 2.f },
			{ -m_size / 2.f, m_size / 2.f, m_size / 2.f },
			{ m_size / 2.f, m_size / 2.f, m_size / 2.f },
			{ m_size / 2.f, -m_size / 2.f, -m_size / 2.f },
			{ -m_size / 2.f, -m_size / 2.f, -m_size / 2.f },
			{ -m_size / 2.f, m_size / 2.f, -m_size / 2.f },
			{ -m_size / 2.f, m_size / 2.f, m_size / 2.f },
			{ -m_size / 2.f, -m_size / 2.f, m_size / 2.f },
			{ m_size / 2.f, -m_size / 2.f, m_size / 2.f },
			{ m_size / 2.f, m_size / 2.f, m_size / 2.f },
			{ m_size / 2.f, -m_size / 2.f, m_size / 2.f },
			{ m_size / 2.f, -m_size / 2.f, -m_size / 2.f },
			{ m_size / 2.f, -m_size / 2.f, m_size / 2.f },
			{ -m_size / 2.f, -m_size / 2.f, m_size / 2.f },
			{ -m_size / 2.f, -m_size / 2.f, -m_size / 2.f },
			{ -m_size / 2.f, -m_size / 2.f, -m_size / 2.f },
			{ -m_size / 2.f, -m_size / 2.f, m_size / 2.f },
			{ -m_size / 2.f, m_size / 2.f, m_size / 2.f },
			{ m_size / 2.f, m_size / 2.f, -m_size / 2.f },
			{ -m_size / 2.f, m_size / 2.f, -m_size / 2.f },
			{ -m_size / 2.f, m_size / 2.f, m_size / 2.f }
		};

		col = new BufferObject<glm::tvec4<uint8_t>>(IBufferObject::USAGE_HINT::STATIC);


		col->data.resize(36, m_color);

		normals = new BufferObject<glm::vec3>(IBufferObject::USAGE_HINT::STATIC);

		normals->data = {
			{ -0.000000, -0.000000, -1.000000 },
			{ -0.000000, -0.000000, -1.000000 },
			{ -0.000000, -0.000000, -1.000000 },
			{ 0.000000, 0.000000, 1.000000 },
			{ 0.000000, 0.000000, 1.000000 },
			{ 0.000000, 0.000000, 1.000000 },
			{ 1.000000, 0.000000, -0.000000 },
			{ 1.000000, 0.000000, -0.000000 },
			{ 1.000000, 0.000000, -0.000000 },
			{ -0.000000, -1.000000, -0.000000 },
			{ -0.000000, -1.000000, -0.000000 },
			{ -0.000000, -1.000000, -0.000000 },
			{ -1.000000, -0.000000, 0.000000 },
			{ -1.000000, -0.000000, 0.000000 },
			{ -1.000000, -0.000000, 0.000000 },
			{ 0.000001, 1.000000, 0.000001 },
			{ 0.000001, 1.000000, 0.000001 },
			{ 0.000001, 1.000000, 0.000001 },
			{ 0.000000, 0.000000, -1.000000 },
			{ 0.000000, 0.000000, -1.000000 },
			{ 0.000000, 0.000000, -1.000000 },
			{ 0.000000, -0.000000, 1.000000 },
			{ 0.000000, -0.000000, 1.000000 },
			{ 0.000000, -0.000000, 1.000000 },
			{ 1.000000, -0.000001, 0.000001 },
			{ 1.000000, -0.000001, 0.000001 },
			{ 1.000000, -0.000001, 0.000001 },
			{ -0.000000, -1.000000, 0.000000 },
			{ -0.000000, -1.000000, 0.000000 },
			{ -0.000000, -1.000000, 0.000000 },
			{ -1.000000, -0.000000, -0.000000 },
			{ -1.000000, -0.000000, -0.000000 },
			{ -1.000000, -0.000000, -0.000000 },
			{ -0.000000, 1.000000, 0.000000 },
			{ -0.000000, 1.000000, 0.000000 },
			{ -0.000000, 1.000000, 0.000000 }
		};

		indices = new IndexBufferObject<uint32_t>(IBufferObject::USAGE_HINT::STATIC);

		indices->data = {
			0,  1,  2,
			3,  4,  5,
			6,  7,  8,
			9,  10, 11,
			12, 13, 14,
			15, 16, 17,
			18, 19, 20,
			21, 22, 23,
			24, 25, 26,
			27, 28, 29,
			30, 31, 32,
			33, 34, 35
		};

		glmesh->buffers[Mesh::POSITION] = pos;
		glmesh->buffers[Mesh::COLOR] = col;
		glmesh->buffers[Mesh::INDICES] = indices;
		glmesh->buffers[Mesh::NORMAL] = normals;

		glmesh->Init();
		return true;
	}

	bool Init(const AABB & aabb)
	{
		glmesh = share(new Mesh());

		pos = new BufferObject<glm::vec3>(IBufferObject::USAGE_HINT::STATIC);

		const glm::vec3 bmin = aabb.GetMin(), bmax = aabb.GetMax();
		/// - - +
		glm::vec3 p1(bmin.x, bmin.y, bmax.z);
		/// + - +
		glm::vec3 p2(bmax.x, bmin.y, bmax.z);
		/// + + +
		glm::vec3 p3(bmax.x, bmax.y, bmax.z);
		/// - + +
		glm::vec3 p4(bmin.x, bmax.y, bmax.z);

		/// - - -
		glm::vec3 p5(bmin.x, bmin.y, bmin.z);
		/// + - -
		glm::vec3 p6(bmax.x, bmin.y, bmin.z);
		/// + + -
		glm::vec3 p7(bmax.x, bmax.y, bmin.z);
		/// - + -
		glm::vec3 p8(bmin.x, bmax.y, bmin.z);

		pos->data.resize(8);
		pos->data[0] = p1;
		pos->data[1] = p2;
		pos->data[2] = p3;
		pos->data[3] = p4;
		pos->data[4] = p5;
		pos->data[5] = p6;
		pos->data[6] = p7;
		pos->data[7] = p8;

		col = new BufferObject<glm::tvec4<uint8_t>>(IBufferObject::USAGE_HINT::STATIC);

		col->data.resize(8);
		col->data[0] = m_color;
		col->data[1] = m_color;
		col->data[2] = m_color;
		col->data[3] = m_color;
		col->data[4] = m_color;
		col->data[5] = m_color;
		col->data[6] = m_color;
		col->data[7] = m_color;

		indices = new IndexBufferObject<uint32_t>(IBufferObject::USAGE_HINT::STATIC);

		indices->data.resize(36);
		// Front
		indices->data[0] = 0;
		indices->data[1] = 1;
		indices->data[2] = 2;
		indices->data[3] = 2;
		indices->data[4] = 3;
		indices->data[5] = 0;
		// Top
		indices->data[6] = 3;
		indices->data[7] = 2;
		indices->data[8] = 6;
		indices->data[9] = 6;
		indices->data[10] = 7;
		indices->data[11] = 3;
		// Back
		indices->data[12] = 7;
		indices->data[13] = 6;
		indices->data[14] = 5;
		indices->data[15] = 5;
		indices->data[16] = 4;
		indices->data[17] = 7;
		// Bottom
		indices->data[18] = 4;
		indices->data[19] = 5;
		indices->data[20] = 1;
		indices->data[21] = 1;
		indices->data[22] = 0;
		indices->data[23] = 4;
		// Left
		indices->data[24] = 4;
		indices->data[25] = 0;
		indices->data[26] = 3;
		indices->data[27] = 3;
		indices->data[28] = 7;
		indices->data[29] = 4;
		// Right
		indices->data[30] = 1;
		indices->data[31] = 5;
		indices->data[32] = 6;
		indices->data[33] = 6;
		indices->data[34] = 2;
		indices->data[35] = 1;

		glmesh->buffers[Mesh::POSITION] = pos;
		glmesh->buffers[Mesh::COLOR] = col;
		glmesh->buffers[Mesh::INDICES] = indices;

		glmesh->Init();
		return true;
	}

	bool Init(float left, float right, float bottom, float top, float znear, float zfar)
	{
		glmesh = share(new Mesh());

		pos = new BufferObject<glm::vec3>(IBufferObject::USAGE_HINT::STATIC);

		/// - - +
		glm::vec3 p1(left, bottom, zfar);
		/// + - +
		glm::vec3 p2(right, bottom, zfar);
		/// + + +
		glm::vec3 p3(right, top, zfar);
		/// - + +
		glm::vec3 p4(left, top, zfar);

		/// - - -
		glm::vec3 p5(left, bottom, znear);
		/// + - -
		glm::vec3 p6(right, bottom, znear);
		/// + + -
		glm::vec3 p7(right, top, znear);
		/// - + -
		glm::vec3 p8(left, top, znear);

		pos->data.resize(8);
		pos->data[0] = p1;
		pos->data[1] = p2;
		pos->data[2] = p3;
		pos->data[3] = p4;
		pos->data[4] = p5;
		pos->data[5] = p6;
		pos->data[6] = p7;
		pos->data[7] = p8;

		col = new BufferObject<glm::tvec4<uint8_t>>(IBufferObject::USAGE_HINT::STATIC);

		col->data.resize(8);
		col->data[0] = m_color;
		col->data[1] = m_color;
		col->data[2] = m_color;
		col->data[3] = m_color;
		col->data[4] = m_color;
		col->data[5] = m_color;
		col->data[6] = m_color;
		col->data[7] = m_color;

		indices = new IndexBufferObject<uint32_t>(IBufferObject::USAGE_HINT::STATIC);

		indices->data.resize(36);
		// Front
		indices->data[0] = 0;
		indices->data[1] = 1;
		indices->data[2] = 2;
		indices->data[3] = 2;
		indices->data[4] = 3;
		indices->data[5] = 0;
		// Top
		indices->data[6] = 3;
		indices->data[7] = 2;
		indices->data[8] = 6;
		indices->data[9] = 6;
		indices->data[10] = 7;
		indices->data[11] = 3;
		// Back
		indices->data[12] = 7;
		indices->data[13] = 6;
		indices->data[14] = 5;
		indices->data[15] = 5;
		indices->data[16] = 4;
		indices->data[17] = 7;
		// Bottom
		indices->data[18] = 4;
		indices->data[19] = 5;
		indices->data[20] = 1;
		indices->data[21] = 1;
		indices->data[22] = 0;
		indices->data[23] = 4;
		// Left
		indices->data[24] = 4;
		indices->data[25] = 0;
		indices->data[26] = 3;
		indices->data[27] = 3;
		indices->data[28] = 7;
		indices->data[29] = 4;
		// Right
		indices->data[30] = 1;
		indices->data[31] = 5;
		indices->data[32] = 6;
		indices->data[33] = 6;
		indices->data[34] = 2;
		indices->data[35] = 1;

		glmesh->buffers[Mesh::POSITION] = pos;
		glmesh->buffers[Mesh::COLOR] = col;
		glmesh->buffers[Mesh::INDICES] = indices;

		glmesh->Init();
		return true;
	}

	void Render(bool wireframe, bool ccw_winding = true)
	{
		if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (!ccw_winding) glFrontFace(GL_CW);
		glmesh->Render();
		if (!ccw_winding) glFrontFace(GL_CCW);
		if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
};

#endif // CUBEMESH_H
