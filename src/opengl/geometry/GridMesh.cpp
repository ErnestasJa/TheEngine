#include "Precomp.h"

#include "GridMesh.h"

GridMesh::GridMesh(float density, uint32_t size, uint32_t gridlines, bool positiveOnly)
{
	BufferObject<glm::vec3> *pos = new BufferObject<glm::vec3>(IBufferObject::USAGE_HINT::STATIC);
	BufferObject<glm::vec4> *col = new BufferObject<glm::vec4>(IBufferObject::USAGE_HINT::STATIC);

	int32_t halfsize = size / 2;

	/// XAxis
	if (!positiveOnly)
	{
		pos->data.push_back(glm::vec3(-halfsize, 0, 0));
		pos->data.push_back(glm::vec3(0, 0, 0));
		col->data.push_back(glm::vec4(1, 0.5, 0.5, 1));
		col->data.push_back(glm::vec4(1, 0.5, 0.5, 1));
	}

	pos->data.push_back(glm::vec3(0, 0, 0));
	pos->data.push_back(glm::vec3(halfsize, 0, 0));
	col->data.push_back(glm::vec4(1, 0, 0, 1));
	col->data.push_back(glm::vec4(1, 0, 0, 1));

	/// YAxis
	if (!positiveOnly)
	{
		pos->data.push_back(glm::vec3(0, -halfsize, 0));
		pos->data.push_back(glm::vec3(0, 0, 0));
		col->data.push_back(glm::vec4(0.5, 1, 0.5, 1));
		col->data.push_back(glm::vec4(0.5, 1, 0.5, 1));
	}

	pos->data.push_back(glm::vec3(0, 0, 0));
	pos->data.push_back(glm::vec3(0, halfsize, 0));
	col->data.push_back(glm::vec4(0, 1, 0, 1));
	col->data.push_back(glm::vec4(0, 1, 0, 1));

	/// ZAxis
	if (!positiveOnly)
	{
		pos->data.push_back(glm::vec3(0, 0, -halfsize));
		pos->data.push_back(glm::vec3(0, 0, 0));
		col->data.push_back(glm::vec4(0.5, 0.5, 1, 1));
		col->data.push_back(glm::vec4(0.5, 0.5, 1, 1));
	}

	pos->data.push_back(glm::vec3(0, 0, 0));
	pos->data.push_back(glm::vec3(0, 0, halfsize));
	col->data.push_back(glm::vec4(0, 0, 1, 1));
	col->data.push_back(glm::vec4(0, 0, 1, 1));

	uint32_t end = positiveOnly ? halfsize : size;

	for (uint32_t i = 0; i <= end / density; i++)
	{
		if (i != halfsize)
		{
			if (!positiveOnly)
			{
				pos->data.push_back(glm::vec3(-halfsize + i*density, 0, -halfsize));
				pos->data.push_back(glm::vec3(-halfsize + i*density, 0, halfsize));

				pos->data.push_back(glm::vec3(-halfsize, 0, -halfsize + i*density));
				pos->data.push_back(glm::vec3(halfsize, 0, -halfsize + i*density));
			}
			else
			{
				if (i != 0)
				{
					pos->data.push_back(glm::vec3(i*density, 0, 0));
					pos->data.push_back(glm::vec3(i*density, 0, halfsize));

					pos->data.push_back(glm::vec3(0, 0, i*density));
					pos->data.push_back(glm::vec3(halfsize, 0, i*density));
				}
			}

			if (i%gridlines != 0)
			{
				col->data.push_back(glm::vec4(0.5, 0.5, 0.5, 1));
				col->data.push_back(glm::vec4(0.5, 0.5, 0.5, 1));

				col->data.push_back(glm::vec4(0.5, 0.5, 0.5, 1));
				col->data.push_back(glm::vec4(0.5, 0.5, 0.5, 1));
			}
			else
			{
				if (i != 0)
				{
					col->data.push_back(glm::vec4(0.85, 0.85, 0.85, 1));
					col->data.push_back(glm::vec4(0.85, 0.85, 0.85, 1));

					col->data.push_back(glm::vec4(0.85, 0.85, 0.85, 1));
					col->data.push_back(glm::vec4(0.85, 0.85, 0.85, 1));
				}
			}
		}
		else if (i == size)
		{
			if (!positiveOnly)
			{
				pos->data.push_back(glm::vec3(-halfsize + i*density, 0, -halfsize));
				pos->data.push_back(glm::vec3(-halfsize + i*density, 0, halfsize));
			}
			else
			{
				pos->data.push_back(glm::vec3(i*density, 0, 0));
				pos->data.push_back(glm::vec3(i*density, 0, halfsize));
			}
			col->data.push_back(glm::vec4(0.75, 0.75, 0.75, 1));
			col->data.push_back(glm::vec4(0.75, 0.75, 0.75, 1));
		}
	}

	this->buffers[Mesh::POSITION] = pos;
	this->buffers[Mesh::COLOR] = col;
	this->Init();
}

GridMesh::~GridMesh()
{

}