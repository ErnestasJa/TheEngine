#pragma once

#include "gui/GUIElement.h"

class Quad;
class Shader;
template <typename T>
class Rect2D;

class GUIStaticText;
class GUIImage;
class GUIButton :public GUIElement
{
private:
	uint32_t _style;
	glm::vec4 _colorActive, _colorHover, _colorClicked, _colorDisabled, _colorText, _colorCurrent;
	bool _colored, _toggled, _toggling;

	GUIImage* _imageOverlay;
	GUIStaticText* _textOverlay;
public:
	GUIButton(GUIEnvironment* env, Rect2D<int> dimensions, std::wstring text = L"text", bool colored=false, bool toggling = false, bool toggleStatus = false);
	virtual ~GUIButton();

	bool IsToggled();
	void SetToggled(bool toggleStatus);
	void SetToggleable(bool toggling);

	void Render();

	void SetText(const std::wstring &text);

	void SetImage(GUIImage *image);
	void RemoveImage();

	void SetColor(const glm::vec4 & colorActive);
	void SetColors(const glm::vec4 & colorActive, const glm::vec4 & colorHover, const glm::vec4 & colorClicked, const glm::vec4 & colorDisabled);

	virtual bool OnEvent(const GUIEvent & e);
protected:
};
