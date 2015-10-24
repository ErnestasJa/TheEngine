#pragma once

#include "gui/GUIElement.h"

template <typename T>
class Rect2D;
class GUIStaticText :public GUIElement
{
private:
	std::wstring m_text;
	bool m_draw_background;

	std::string _font;
public:
	GUIStaticText(GUIEnvironment* env, Rect2D<int> dimensions, std::wstring text = L"text", bool drawbackground = false);
	virtual ~GUIStaticText();

	void Render();

	void SetText(const std::wstring &text);
	void SetFont(const std::string &fontName);

	const std::wstring &get_text()
	{
		return m_text;
	}

	//this does not do any events
	virtual bool OnEvent(const GUIEvent & e)
	{
		return false;
	}
protected:
};
