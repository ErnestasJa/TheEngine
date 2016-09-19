#include "Precomp.h"
#include "GUIGraph.h"
#include "GUI.h"

#include "opengl/Texture.h"
#include "resources/Image.h"

GUIGraph::GUIGraph(GUIEnvironment * env, Rect2D<int> dimensions) :GUIElement(env, dimensions)
{
	this->Type = GUIET_IMAGE;

	img = share(new Image());
	img->Init(dimensions.w, dimensions.h, 4);
	tex = share(new Texture());
	tex->Init(img);
	graphImage = new GUIImage(env, dimensions, tex);
	graphImage->SetParent(this);
}

GUIGraph::~GUIGraph()
{
}

void GUIGraph::AddValue(float value)
{
	if (values.size() > relative_rect.w)
		values.pop_back();
	values.insert(values.begin(), value);
}

void GUIGraph::Update(float dt)
{
}

void GUIGraph::Render()
{
	RenderChildren();
}

void GUIGraph::SetScale(const glm::vec2 & axisScales)
{
}

void GUIGraph::UpdateImage()
{
	if (values.size() > 1)
	{
		for (uint32_t i = 0; i < values.size() - 1; i++)
		{
			auto value1 = 128+values[i];
			auto value2 = 128+values[i + 1];

			DrawLine(i, value1, i + 1, value2, glm::vec4(255));
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
			else
			{
				y = y + s2;
				d = d - 2 * dx;
			}
		}
		if (swap) y = y + s2;
		else x = x + s1;
		d = d + 2 * dy;
	}
	UpdateImage();
}