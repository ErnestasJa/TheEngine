#include "Precomp.h"
#include "gui/GUI.h"
#include "resources/Image.h"
#include "opengl/Texture.h"
#include "GUIColorSwatches.h"
#include "GUIColorPicker.h"
#include "application/AppContext.h"
#include "core/FileSystem.h"

static const char* swatchesFilename = "swatches.dat";

GUIColorSwatches::GUIColorSwatches(GUIEnvironment * env, Rect2D<int> dimensions, const glm::ivec2 & swatchSize, GUIColorPicker* picker) :GUIElement(env, dimensions)
{
	this->Type = GUIET_PANE;

	xSize = glm::ceil((float)dimensions.w / (float)swatchSize.x);
	ySize = glm::ceil((float)dimensions.h / (float)swatchSize.y);

	this->picker = picker;

	swatches.resize(xSize*ySize, glm::vec4(0));

	this->swatchSize = swatchSize;

	imgBuf = share(new Image());
	imgBuf->Init(dimensions.w, dimensions.h, 3);
	texBuf = share(new Texture());
	texBuf->Init(imgBuf);

	swatchImage = new GUIImage(env, Rect2D<int>(0, 0, dimensions.w, dimensions.h), texBuf);
	swatchImage->SetParent(this);
	swatchImage->SetListening(false);
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
	auto file = GetContext().GetFileSystem()->OpenWrite(swatchesFilename);
	uint32_t size = swatches.size();
	file->Write((void*)&size, sizeof(uint32_t));
	file->Write((void*)(&swatches[0]), sizeof(glm::vec4) * size);
}

void GUIColorSwatches::LoadSwatches()
{
	auto fs = GetContext().GetFileSystem();

	if (fs->FileExists(swatchesFilename))
	{
		auto file = fs->OpenRead("swatches.dat");
		uint32_t size = ((uint32_t*)file->Read(sizeof(uint32_t))->data())[0];
		swatches.resize(size);
		loopi(size)
		{
			auto data = ((glm::vec4*)file->Read(sizeof(glm::vec4))->data())[0];
			swatches[i] = data;
		}
		UpdateSwatchImage();
	}
}

void GUIColorSwatches::UpdateSwatchImage()
{
	loop(y, ySize)
	{
		loop(x, xSize)
		{
			auto index = y*xSize + x;
			auto swatchColor = swatches[index];

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
						auto oldcol = swatchColor;
						swatchColor = glm::vec4(0, 0, 0, 255);
						imgBuf->SetPixel(ix, iy, swatchColor.r, swatchColor.g, swatchColor.b, swatchColor.a);
						swatchColor = oldcol;
					}
					else
					{
						imgBuf->SetPixel(ix, iy, swatchColor.r, swatchColor.g, swatchColor.b, swatchColor.a);
					}
				}
			}
		}
	}

	texBuf->SetSubImage2D(imgBuf->data, 0, 0, imgBuf->width, imgBuf->height);
}

bool GUIColorSwatches::OnEvent(const GUIEvent &e)
{
	bool mousePressed = false;
	bool right = false;
	GUI_BEGIN_ON_EVENT(e)
		switch (e.GetType())
		{
		case gui_event_type::right_mouse_pressed:
			mousePressed = true;
			right = true;
			break;
		case gui_event_type::left_mouse_pressed:
			mousePressed = true;
			break;
		}

	if (hovered && mousePressed)
	{
		auto mousePos = environment->GetMousePosition();
		auto modx = (int)glm::floor((mousePos.x - absolute_rect.x) / swatchSize.x);
		auto mody = (int)glm::floor((absolute_rect.h - (mousePos.y - absolute_rect.y)) / swatchSize.y);
		auto pixelCoord = glm::vec2(modx*swatchSize.x + swatchSize.x / 2, mody*swatchSize.y + swatchSize.y / 2);
		color = imgBuf->GetPixel(pixelCoord.x, pixelCoord.y);

		if (picker)
		{
			if (right)
			{
				picker->SetSecondaryColorRGBA(color.r, color.g, color.b, color.a);
			}
			else
			{
				picker->SetPrimaryColorRGBA(color.r, color.g, color.b, color.a);
			}
		}
	}

	GUI_END_ON_EVENT(e)
}