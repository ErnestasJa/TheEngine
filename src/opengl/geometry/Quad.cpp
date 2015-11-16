#include "Precomp.h"

#include "../OpenglInc.h"
#include "Quad.h"

Quad::Quad(float size)
{
	this->m_size = size;

	auto posBuffer = new BufferObject<glm::vec3>();
	posBuffer->data.resize(4);
	posBuffer->data[0] = glm::vec3(-m_size, -m_size, 0);
	posBuffer->data[1] = glm::vec3(m_size, -m_size, 0);
	posBuffer->data[2] = glm::vec3(m_size, m_size, 0);
	posBuffer->data[3] = glm::vec3(-m_size, m_size, 0);

	auto UVBuffer = new BufferObject<glm::vec2>();
	UVBuffer->data.resize(4);
	UVBuffer->data[0] = glm::vec2(0, 0);
	UVBuffer->data[1] = glm::vec2(1, 0);
	UVBuffer->data[2] = glm::vec2(1, 1);
	UVBuffer->data[3] = glm::vec2(0, 1);

	auto indexBuffer = new IndexBufferObject<uint32_t>();
	indexBuffer->data.resize(6);
	indexBuffer->data[0] = 3;
	indexBuffer->data[1] = 0;
	indexBuffer->data[2] = 1;
	indexBuffer->data[3] = 1;
	indexBuffer->data[4] = 2;
	indexBuffer->data[5] = 3;

	buffers[Mesh::POSITION] = posBuffer;
	buffers[Mesh::TEXCOORD] = UVBuffer;
	buffers[Mesh::INDICES] = indexBuffer;

	Init();
}

void Quad::SetUV(vector<glm::vec2> uvs)
{
	auto buffer = static_cast<BufferObject<glm::vec2>* >(buffers[Mesh::TEXCOORD]);
	buffer->data = uvs;
	UploadBufferSubData(Mesh::BUFFER_OBJECT_INDEX::TEXCOORD, uvs, 0);
}