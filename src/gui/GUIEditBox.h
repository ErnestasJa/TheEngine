#pragma once

#include "gui/GUIElement.h"

template <typename T>
class Rect2D;
class GUIEditBox :public GUIElement
{
private:
	uint32_t curspos, reptimer, font_size, maxlength;
	float blinktimer;
	int32_t sx;
	float _mx, _mw, _my, _mh; //margins for drawing
	wchar_t lastkey;
	bool blink, clearonsubmit;
	std::wstring m_text, disp_text;
	glm::vec4 m_text_color;

	gui_style cur_style;

	void add_text(int32_t index, std::wstring text);
	void remove_text(int32_t index, int32_t length);
public:
	GUIEditBox(GUIEnvironment* env, Rect2D<int> dimensions, std::wstring text = L"text", glm::vec4 text_color = glm::vec4(1, 1, 1, 1), bool drawbackground = false, bool drawshadow = false, bool clearonsubmit = false);
	virtual ~GUIEditBox();

	void Render();
	virtual void Update(float dt);

	void SetText(const std::wstring &text);

	const std::wstring &get_text()
	{
		return m_text;
	}

	void SetMaxLength(uint32_t length = 0);

	bool OnEvent(const GUIEvent & e);
protected:
};
