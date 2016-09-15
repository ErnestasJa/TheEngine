#include "Precomp.h"
#include "gui/GUI.h"
#include "resources/Image.h"
#include "opengl/Texture.h"
#include "GUIColorSwatches.h"
#include "application/AppContext.h"
#include "core/FileSystem.h"

GUIColorSwatches::GUIColorSwatches(GUIEnvironment * env, Rect2D<int> dimensions, const glm::ivec2 & swatchSize) :GUIElement(env, dimensions)
{
	this->Type = GUIET_PANE;

	xSize = glm::ceil((float)dimensions.w / (float)swatchSize.x);
	ySize = glm::ceil((float)dimensions.h / (float)swatchSize.y);

	swatches.resize(xSize*ySize, glm::vec4(0));

	this->swatchSize = swatchSize;

	imgBuf = share(new Image());
	imgBuf->Init(dimensions.w, dimensions.h, 3);
	texBuf = share(new Texture());
	texBuf->Init(imgBuf);

	swatchImage = new GUIImage(env, Rect2D<int>(16, 16, dimensions.w, dimensions.h), texBuf);
	swatchImage->SetParent(this);
	swatchImage->SetListening(true);
}

GUIColorSwatches::~GUIColorSwatches()
{
	SaveSwatches();
}

void GUIColorSwatches::AddSwatchColor(const glm::vec4 & color)
{
	if (std::find(swatches.begin(), swatches.end(), color) == swatches.end())
	{
		swatches.pop_back();
		swatches.insert(swatches.begin(), color);
		UpdateSwatchImage();
	}
}

void GUIColorSwatches::Render()
{
	RenderChildren();
}

void GUIColorSwatches::SaveSwatches()
{
	auto file = GetContext().GetFileSystem()->OpenWrite("swatches.dat");
	uint32_t size = swatches.size();
	file->Write((void*)&size, sizeof(uint32_t));
	file->Write((void*)(&swatches[0]), sizeof(glm::vec4) * size);
}

void GUIColorSwatches::LoadSwatches()
{
	auto file = GetContext().GetFileSystem()->OpenRead("swatches.dat");
	uint32_t size = ((uint32_t*)file->Read(sizeof(uint32_t))->data())[0];
	swatches.resize(size);
	loopi(size)
	{
		auto data = ((glm::vec4*)file->Read(sizeof(glm::vec4))->data())[0];
		swatches[i] = data;
	}
	UpdateSwatchImage();
}

void GUIColorSwatches::UpdateSwatchImage()
{
	loop(y, ySize)
	{
		loop(x, xSize)
		{
			auto index = y*xSize + x;
			auto color = swatches[index];

			auto pixelStartX = x*swatchSize.x;
			auto pixelStartY = (ySize*swatchSize.y - swatchSize.y) - y*swatchSize.y;

			auto pixelEndX = pixelStartX + swatchSize.x;
			auto pixelEndY = pixelStartY + swatchSize.y;

			for (uint32_t iy = pixelStartY; iy < pixelEndY; iy++)
			{
				for (uint32_t ix = pixelStartX; ix < pixelEndX; ix++)
				{
					if (iy == pixelStartY || ix == pixelStartX)
					{
						auto oldcol = color;
						color = glm::vec4(0, 0, 0, 255);
						imgBuf->SetPixel(ix, iy, color.r, color.g, color.b, color.a);
						color = oldcol;
					}
					else
					{
						imgBuf->SetPixel(ix, iy, color.r, color.g, color.b, color.a);
					}
				}
			}
		}
	}

	texBuf->SetSubImage2D(imgBuf->data, 0, 0, imgBuf->width, imgBuf->height);
}

bool GUIColorSwatches::OnEvent(const GUIEvent &e)
{
	GUI_BEGIN_ON_EVENT(e)

	GUI_END_ON_EVENT(e)
}