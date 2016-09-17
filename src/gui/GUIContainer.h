#pragma once

#include "GUIElement.h"

struct GUIContainerCell
{
	glm::ivec2 margin, padding;
	glm::ivec2 coords, size;
	bool fit;
};

class GUIContainer
{
public:
	GUIContainer(GUIElement* parent, const glm::vec2 &splits)
	{
		auto parentDims = parent->GetRelativeRect();
		_size = glm::ivec2(parentDims.w, parentDims.h);
		auto cellsX = _size.x / splits.x;
		auto cellsY = _size.y / splits.y;
		_cellSize = glm::ivec2(cellsX, cellsY);
	}
	GUIContainer(GUIContainer* parent, const glm::vec2 &splits);
	~GUIContainer();

	GUIContainer* GetContainer(const glm::ivec2 &cell);
	GUIContainerCell* GetCellInfo();
	void RecalculateCells()
	{

	}
	vector<GUIContainerCell> _cells;
	vector<GUIContainer*> _children;
	glm::ivec2 _size;
	glm::ivec2 _cellSize;
};

