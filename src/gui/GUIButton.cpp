#include "Precomp.h"

#include "opengl/geometry/Quad.h"
#include "GUIEnvironment.h"
#include "GUIButton.h"
#include "GUIStaticText.h"
#include "GUISkin.h"
#include "GUIImage.h"

GUIButton::GUIButton(GUIEnvironment* env, Rect2D<int> dimensions, std::wstring text, bool colored, bool toggle, bool toggleStatus) : GUIElement(env, dimensions)
{
	this->Type = GUIET_BUTTON;

	absolute_rect = dimensions;
	relative_rect = absolute_rect;

	_style = gui_skin_button_active;

	_colored = colored;
	_toggled = toggleStatus;
	_toggling = toggle;

	_imageOverlay = nullptr;

	glm::vec2 textDim = this->environment->GetFontRenderer()->GetTextDimensions(text);
	//printf("Text dim X: %f Y: %f\n", textDim.x, textDim.y);
	_textOverlay = env->AddGUIStaticText(Rect2D<int>(0, 0, textDim.x, textDim.y), text, false);
	_textOverlay->SetParent(this);
	_textOverlay->SetAlignment(HALIGN_CENTER, VALIGN_CENTER);
	_textOverlay->SetListening(false);
}

GUIButton::~GUIButton()
{
}

void GUIButton::Render()
{
	if (_colored)
	{
		environment->draw_gui_quad(absolute_rect, _colorCurrent);
	}
	else
	{
		environment->draw_sliced_gui_quad(absolute_rect, _toggled&&_toggling ? gui_skin_button_click : enabled ? _style : gui_skin_button_disabled);
	}

	RenderChildren();
}

void GUIButton::SetText(const std::wstring &text)
{
	_textOverlay->SetText(text);
	_textOverlay->UpdateAbsolutePos();
}

void GUIButton::SetImage(GUIImage* img)
{
	if (_imageOverlay != nullptr)
	{
		RemoveImage();
	}
	else
	{
		_imageOverlay = img;
		AddChild(img);
	}
}

void GUIButton::RemoveImage()
{
	this->RemoveChild(_imageOverlay);
	_imageOverlay = nullptr;
}

bool GUIButton::OnEvent(const GUIEvent & e)
{
	GUI_BEGIN_ON_EVENT(e)
		switch (e.GetType())
		{
		case gui_event_type::element_hovered:
			_style = gui_skin_button_hover;
			GUI_FIRE_EVENT(GUIEvent(element_hovered, this, this))
				break;

		case gui_event_type::element_exitted:
			_style = gui_skin_button_active;
			GUI_FIRE_EVENT(GUIEvent(element_exitted, this, this))
				break;

		case gui_event_type::mouse_pressed:
			_style = gui_skin_button_click;
			GUI_FIRE_EVENT(GUIEvent(button_pressed, this, this))
				break;

		case gui_event_type::mouse_released:
			if (_toggling)
				_toggled = !_toggled;
			_style = hovered ? gui_skin_button_hover : gui_skin_button_active;
			GUI_FIRE_EVENT(GUIEvent(button_released, this, this))
				break;

		default:
			break;
		}
	GUI_END_ON_EVENT(e)
}

bool GUIButton::IsToggled()
{
	return _toggled;
}

void GUIButton::SetToggled(bool toggleStatus)
{
	_toggled = toggleStatus;
}

void GUIButton::SetToggleable(bool toggling)
{
	_toggling = toggling;
}

void GUIButton::SetColor(const glm::vec4 & colorActive)
{
	_colorCurrent = colorActive;
}
void GUIButton::SetColors(const glm::vec4 & colorActive, const glm::vec4 & colorHover, const glm::vec4 & colorClicked, const glm::vec4 & colorDisabled)
{
	_colorActive = colorActive;
	_colorHover = colorHover;
	_colorClicked = colorClicked;
	_colorDisabled = colorDisabled;
}