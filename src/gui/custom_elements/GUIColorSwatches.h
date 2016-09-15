#pragma once

#include "ForwardDecl.h"
#include "gui/GUIElement.h"

class GUIImage;

class GUIColorSwatches :public GUIElement
{
private:
	GUIImage* swatchImage;
	ImagePtr imgBuf;
	TexturePtr texBuf;

	int xSize, ySize;

	glm::vec4 color;

	vector<glm::vec4> swatches;
	glm::ivec2 swatchSize;
public:
	GUIColorSwatches(GUIEnvironment* env, Rect2D<int> dimensions, const glm::ivec2 &swatchSize);
	virtual ~GUIColorSwatches();

	void AddSwatchColor(const glm::vec4 &color);
	glm::vec4 GetActiveSwatchColor();

	void Render();

	void SaveSwatches();
	void LoadSwatches();

	virtual bool OnEvent(const GUIEvent & e);
private:
	void UpdateSwatchImage();
protected:
};

