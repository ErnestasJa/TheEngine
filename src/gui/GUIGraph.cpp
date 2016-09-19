#include "Precomp.h"
#include "GUIGraph.h"
#include "GUI.h"

#include "opengl/Texture.h"
#include "resources/Image.h"

GUIGraph::GUIGraph(GUIEnvironment * env, Rect2D<int> dimensions, bool drawBorders, bool drawCenter, bool drawAxisValues, bool drawBackground, bool drawChart) :GUIElement(env, dimensions)
{
	this->Type = GUIET_IMAGE;

	img = share(new Image());
	img->Init(dimensions.w, dimensions.h, 4);
	tex = share(new Texture());
	tex->Init(img);
	graphImage = new GUIImage(env, Rect2D<int>(0, 0, dimensions.w, dimensions.h), tex);
	graphImage->SetParent(this);

	this->drawBorders = drawBorders;
	this->drawCenter = drawCenter;
	this->drawAxisValues = drawAxisValues;
	this->drawBackground = drawBackground;
	this->drawChart = drawChart;
	dataChanged = false;
}

GUIGraph::~GUIGraph()
{
}

void GUIGraph::AddValueGroup(const std::string &valueName, int spacing, const glm::vec4 &color, bool trackMinMax)
{
	valueGroups[valueName] = vector<float>();
	valueProperties[valueName] = GraphValueProperties(spacing, color, trackMinMax);
	if (trackMinMax)
	{
		valueTexts[valueName] = new GUIStaticText(environment, Rect2D<int>(0, valueTexts.size() * 12, relative_rect.w, 12));
		valueTexts[valueName]->SetParent(this);
	}
}

void GUIGraph::AddValue(const std::string &valueName, float value, int spacing, const glm::vec4 &color, bool trackMinMax)
{
	if (valueGroups.find(valueName) == valueGroups.end())
	{
		AddValueGroup(valueName, spacing, color, trackMinMax);
	}

	auto &realValues = valueGroups[valueName];

	auto &properties = valueProperties[valueName];

	while (realValues.size() * properties.spacing >= relative_rect.w)
	{
		realValues.erase(realValues.begin());
	}
	realValues.push_back(value);

	if (properties.trackMinMax)
	{
		properties.maxValue = glm::max(value, properties.maxValue);
		properties.minValue = glm::min(value, properties.minValue);
		wchar_t buf[256];
		swprintf(buf, L"['s]['c %d %d %d]%s Min:%4.4f Max:%4.4f Current:%4.4f[c'][s']", (int)properties.color.r, (int)properties.color.g, (int)properties.color.b, helpers::to_wstr(valueName.c_str()).c_str(), properties.minValue, properties.maxValue, value);
		valueTexts[valueName]->SetText(buf);
	}
	dataChanged = true;
}

void GUIGraph::SetValueColor(const std::string & valueName, const glm::vec4 & valueColor)
{
	if (valueProperties.find(valueName) != valueProperties.end())
	{
		valueProperties[valueName].color = valueColor;
	}
}

void GUIGraph::Update(float dt)
{
}

void GUIGraph::Render()
{
	if (dataChanged)
	{
		UpdateImage();
		dataChanged = false;
	}
	RenderChildren();
}

void GUIGraph::SetScale(const glm::vec2 & axisScales)
{
}

void GUIGraph::UpdateImage()
{
	loop(x, img->width)
	{
		loop(y, img->height)
		{
			img->SetPixel(x, y, 1, 1, 1, drawBackground ? 128 : 0);
		}
	}

	if (valueGroups.size() > 0)
	{
		auto width = relative_rect.w - 1;
		auto height = relative_rect.h - 1;

		auto center = relative_rect.h / 2 - 1;

		if (drawCenter)
		{
			DrawLine(0, center, relative_rect.w - 1, center, glm::vec4(255, 0, 0, 128));
		}

		glm::vec4 borderColor = glm::vec4(0, 0, 0, 255);
		glm::vec4 markerColor = glm::vec4(255, 255, 255, 255);

		for (auto &group : valueGroups)
		{
			auto &values = group.second;
			auto &properties = valueProperties[group.first];
			auto color = properties.color;
			auto spacing = properties.spacing;

			for (uint32_t i = 0; i < values.size() - 1; i++)
			{
				auto value1 = center + values[i];
				auto value2 = center + values[i + 1];

				value1 = glm::clamp<int>(value1, 0, relative_rect.h - 1);
				value2 = glm::clamp<int>(value2, 0, relative_rect.h - 1);


				DrawLine(i * spacing, value1, (i + 1) * spacing, value2, color);
				if (drawChart)
				{
					DrawLine(i*spacing, 0, i*spacing, value1, color);
				}

				if (drawAxisValues)
				{
					DrawLine(i * spacing, 0, i*spacing, 8, markerColor);
					DrawLine((i + 1) * spacing, 0, (i + 1)*spacing, 8, markerColor);
				}
			}
		}

		if (drawBorders)
		{
			DrawLine(0, height, width, height, borderColor);
			DrawLine(width, 0, width, height, borderColor);
			DrawLine(0, 0, width, 0, borderColor);
			DrawLine(0, 0, 0, height, borderColor);
		}
	}
	tex->SetSubImage2D(img->data, 0, 0, img->width, img->height);
}

/* Generalized Bresenham's Algorithm */
void GUIGraph::DrawLine(int x1, int y1, int x2, int y2, const glm::vec4 &color)
{
	int dx, dy, x, y, d, s1, s2, swap = 0, temp;

	dx = glm::abs(x2 - x1);
	dy = glm::abs(y2 - y1);
	s1 = glm::sign(x2 - x1);
	s2 = glm::sign(y2 - y1);

	/* Check if dx or dy has a greater range */
	/* if dy has a greater range than dx swap dx and dy */
	if (dy > dx) { temp = dx; dx = dy; dy = temp; swap = 1; }

	/* Set the initial decision parameter and the initial point */
	d = 2 * dy - dx;
	x = x1;
	y = y1;

	int i;
	for (i = 1; i <= dx; i++)
	{
		img->SetPixel(x, y, color.r, color.g, color.b, color.a);

		while (d >= 0)
		{
			if (swap) x = x + s1;
			else y = y + s2;
			d = d - 2 * dx;
		}
		if (swap) y = y + s2;
		else x = x + s1;
		d = d + 2 * dy;
	}
}