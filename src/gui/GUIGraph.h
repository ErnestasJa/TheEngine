#pragma once

#include "ForwardDecl.h"
#include "GUIElement.h"

class GUIImage;

class GUIGraph :public GUIElement
{
public:
	GUIGraph(GUIEnvironment* env, Rect2D<int> dimensions);
	~GUIGraph();

	void AddValue(float value); //last add time..?

	void Update(float dt);

	void Render();

	void SetScale(const glm::vec2 &axisScales); //units to pixels

	void DrawLine(int startX, int startY, int endX, int endY, const glm::vec4 &color);
private:
	void UpdateImage();

	GUIImage* graphImage;
	ImagePtr img;
	TexturePtr tex;
	vector<float> values;
	float min, max;
protected:
};