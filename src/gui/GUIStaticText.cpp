#include "Precomp.h"

#include "opengl/geometry/Quad.h"
#include "GUIEnvironment.h"
#include "GUIStaticText.h"

GUIStaticText::GUIStaticText(GUIEnvironment* env, Rect2D<int> dimensions, std::wstring text, bool drawbackground) :GUIElement(env, dimensions)
{
	this->Type = GUIET_STATIC_TEXT;
	environment = env;

	m_draw_background = drawbackground;

	absolute_rect = dimensions;
	relative_rect = absolute_rect;

	m_text = text;
	_font = "default";
}

GUIStaticText::~GUIStaticText()
{
}

void GUIStaticText::Render()
{
	if (this->m_draw_background)
	{
		environment->draw_gui_quad(absolute_rect, gui_skin_background);
	}

	environment->GetFontRenderer()->RenderString(this->m_text, glm::ivec2(this->absolute_rect.x, this->absolute_rect.y), 0, _font);

	this->RenderChildren();
}

void GUIStaticText::SetText(const std::wstring &text)
{
	this->m_text = text;
}

void GUIStaticText::SetFont(const std::string &font)
{
	_font = font;
}