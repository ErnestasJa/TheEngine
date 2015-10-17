#pragma once

#include "gui/GUIElement.h"

class GUIButton;
class GUIPane;
class GUIStaticText;

class GUIWindow :public GUIElement
{
private:
	bool _clipping, _showClose, _modal, _dragging, _draggable;

	glm::mat4 m_transform;
	GUIPane *_titlebar, *_background;
	GUIStaticText *_titlebarText;
	GUIButton* _closeButton;

	glm::vec2 _mousePos, _difference, _dragStart;
	void Move(glm::vec2 delta);
public:
	GUIWindow(GUIEnvironment* env, Rect2D<int> dimensions, std::wstring tittleBarText = L"Window", bool clip = true, bool showClose = true, bool modal = false, bool draggable = true);
	virtual ~GUIWindow();

	void Render();

	virtual bool OnEvent(const GUIEvent & e);
protected:
};
typedef std::shared_ptr<GUIWindow> GUIWindowPtr;