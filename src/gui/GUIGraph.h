#pragma once

#include "ForwardDecl.h"
#include "GUIElement.h"

class GUIImage;
class GUIStaticText;

struct GraphValueProperties
{
	glm::vec4 color;
	int spacing;
	bool trackMinMax;
	float minValue, maxValue;

	GraphValueProperties()
	{
		color = glm::vec4(255);
		spacing = 1;
		trackMinMax = false;
		minValue = std::numeric_limits<float>::max();
		maxValue = std::numeric_limits<float>::min();
	}

	GraphValueProperties(int spacing, const glm::vec4 &color, bool trackMinMax)
	{
		this->color = color;
		this->spacing = spacing;
		this->trackMinMax = trackMinMax;
		minValue = std::numeric_limits<float>::max();
		maxValue = std::numeric_limits<float>::min();
	}
};

class GUIGraph :public GUIElement
{
public:
	GUIGraph(GUIEnvironment* env, Rect2D<int> dimensions, bool drawBorders = true, bool drawCenter = true, bool drawAxisValues = true, bool drawBackground = true, bool drawChart = false);
	~GUIGraph();

	void AddValue(const std::string &valueName, float value, int spacing = 1, const glm::vec4 &color = glm::vec4(255), bool trackMinMax = false); //last add time..?
	void SetValueColor(const std::string &valueName, const glm::vec4 &valueColor);
	void Update(float dt);
	void Render();
	void SetScale(const glm::vec2 &axisScales); //units to pixels
	void UpdateImage();
private:
	void AddValueGroup(const std::string &valueName, int spacing = 1, const glm::vec4 &color = glm::vec4(255), bool trackMinMax = false);
	void DrawLine(int startX, int startY, int endX, int endY, const glm::vec4 &color);

	GUIImage* graphImage;
	ImagePtr img;
	TexturePtr tex;
	std::map<std::string, vector<float>> valueGroups;
	std::map<std::string, GraphValueProperties> valueProperties;
	std::map<std::string, GUIStaticText*> valueTexts;
	float minValue, maxValue;
	bool drawChart, drawBorders, drawCenter, drawAxisValues, drawBackground, dataChanged;
protected:
};