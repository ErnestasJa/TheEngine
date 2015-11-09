#include "Precomp.h"
#include "opengl/geometry/Quad.h"
#include "GUI.h"

GUIWindow::GUIWindow(GUIEnvironment* env, Rect2D<int> dimensions, std::wstring tittleBarText, bool clip, bool showClose, bool modal, bool draggable) :GUIElement(env, dimensions)
{
	this->Type = GUIET_WINDOW;

	_clipping = clip;
	_showClose = showClose;
	this->modal = modal;
	if (modal)
	{
		env->SetModal(this);
	}
	_draggable = draggable;

	_dragging = false;

	absolute_rect = dimensions;
	relative_rect = absolute_rect;

	_titlebar = env->AddGUIPane(Rect2D<int>(0, 0, absolute_rect.w, 24), true);
	_titlebar->SetListening(false);
	_titlebar->SetParent(this);

	_background = env->AddGUIPane(Rect2D<int>(0, 24, absolute_rect.w, absolute_rect.h - 24), true);
	_background->SetListening(false);
	_background->SetParent(this);

	auto textDimensions = env->GetFontRenderer()->GetTextDimensions(tittleBarText);
	_titlebarText = env->AddGUIStaticText(Rect2D<int>(0, 0, textDimensions.x, textDimensions.y), tittleBarText);
	_titlebarText->SetParent(_titlebar);
	_titlebarText->SetListening(false);
	_titlebarText->SetAlignment(HALIGN_CENTER, VALIGN_CENTER);

	if (_showClose)
	{
		_closeButton = new GUIButton(env, Rect2D<int>(4, 0, 16, 16), L"['s]X[s']");
		_closeButton->SetParent(_titlebar);
		_closeButton->SetEventListener(this);
		_closeButton->SetAlignment(HALIGN_RIGHT, VALIGN_CENTER);
	}
}

GUIWindow::~GUIWindow()
{
	RemoveChild(_titlebar);
	RemoveChild(_background);
	RemoveChild(_titlebarText);
	RemoveChild(_closeButton);

	delete _titlebar;
	delete _background;
	delete _titlebarText;
	delete _closeButton;
}

void GUIWindow::Render()
{
	this->RenderChildren();
}

bool GUIWindow::OnEvent(const GUIEvent & e)
{
	GUI_BEGIN_ON_EVENT(e)

		switch (e.GetType())
		{
		case left_mouse_pressed:
			_dragStart = environment->GetMousePosition();
			if (_draggable&&_dragging == false && _titlebar->GetAbsoluteRect().is_point_inside((int)_dragStart.x, (int)_dragStart.y) == true)
				_dragging = true;
			break;
		case left_mouse_released:
			if (_dragging) _dragging = false;
			break;
		case mouse_dragged:
			if (_draggable&&_dragging)
			{
				_mousePos = environment->GetMousePosition();
				_difference.x = _mousePos.x - _dragStart.x;
				_difference.y = _mousePos.y - _dragStart.y;
				this->Move(glm::vec2(_difference.x, _difference.y));
			}
			break;
		case mouse_moved:
			_mousePos = environment->GetMousePosition();

			break;
		case button_pressed:

			break;
		case button_released:
			if (e.get_caller() == this->_closeButton)
			{
				this->SetVisible(false);
				GUI_FIRE_EVENT(GUIEvent(window_closed, this, this))
			}

			break;
		default:
			break;
		}

	GUI_END_ON_EVENT(e)
}

void GUIWindow::Move(glm::vec2 delta)
{
	relative_rect.x += delta.x;
	relative_rect.y += delta.y;
	if (_clipping)
	{
		Rect2D<int> par = parent->GetAbsoluteRect();
		relative_rect.clip(par);
	}
	UpdateAbsolutePos();
	_dragStart = environment->GetMousePosition();
}