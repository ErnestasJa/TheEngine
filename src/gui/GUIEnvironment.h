#pragma once

#include "ForwardDecl.h"

#include "GUIElement.h"
#include "GUISkin.h"
#include "font_rendering/FontRenderer.h"

class Quad;
class SlicedGUIQuad;
class ApplicationWindow;
class GUISkin;
class Logger;
class AppContext;

class GUIPane;
class GUIWindow;
class GUIStaticText;
class GUIButton;
class GUIEditBox;
class GUICheckbox;
class GUISlider;
class GUIImage;

class GUIEnvironment : public GUIElement
{
public:
	GUIEnvironment();
	~GUIEnvironment();

	void Render();

	bool IsGUIFocused()
	{
		return hover != nullptr&&hover != this;
	}

	virtual bool OnEvent(const GUIEvent & e);

	void DrawGUIQuad(Rect2D<int> size, std::shared_ptr<Texture> tex, bool tile = false, bool multichannel = true, bool glTex = false);
	void DrawGUIQuad(Rect2D<int> size, uint32_t style = gui_style::gui_skin_background, bool tile = false);
	void DrawGUIQuad(Rect2D<int> size, glm::vec4 col = glm::vec4(1.f));

	void DrawSlicedGUIQuad(Rect2D<int> size, std::shared_ptr<Texture> tex, bool tile = false);
	void DrawSlicedGUIQuad(Rect2D<int> size, uint32_t style = gui_style::gui_skin_background);
	void DrawSlicedGUIQuad(Rect2D<int> size, glm::vec4 col = glm::vec4(1.f));

	glm::vec2 GetMousePosition();
	glm::vec2 GetGUIScaling();

	void OnMouseMove(double x, double y);
	void OnMouseClick(int32_t button, int32_t action, int32_t mod);
	void OnMouseScroll(double sx, double sy);
	void OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t mod);
	void OnCharacterTyped(int32_t scan_code);

	void DestroyChildren();

	GUIElement* GetFocusElement();
	GUIElement* GetHoverElement();
	void SetFocusElement(GUIElement* element);
	void SetHoverElement(GUIElement* element);

	void SetModal(GUIElement* elem)
	{
		if (modal != elem)
		{
			modal = elem;
		}
	}

	void RemoveModal()
	{
		modal = nullptr;
	}

	const std::wstring &GetClipboard()
	{
		return clipboard_string;
	}

	wchar_t GetLastChar()
	{
		return last_char;
	}

	int32_t GetLastKey()
	{
		return last_key;
	}

	template <typename T>
	Rect2D<T> ScaleGUIRect(Rect2D<T> unscaled)
	{
		T gsx = GetGUIScaling().x;
		T gsy = GetGUIScaling().y;
		T px = -1 + unscaled.x*gsx + unscaled.w / 2 * gsx;
		T py = 1 - unscaled.y*gsy - unscaled.h / 2 * gsy;
		T sx = unscaled.w / 2 * gsx;
		T sy = unscaled.h / 2 * gsy;

		Rect2D<T> ret = Rect2D<T>(px, py, sx, sy);

		return ret;
	}

	FontRenderer *GetFontRenderer();

	GUIStaticText* AddGUIStaticText(Rect2D<int> dimensions, std::wstring text = L"text", bool drawbackground = false);
	GUIButton*  AddGUIButton(Rect2D<int> dimensions, std::wstring text = L"text", bool colored = false, bool toggling = false, bool toggleStatus = false);
	GUICheckbox* AddGUICheckbox(Rect2D<int> dimensions, bool checked);
	GUIEditBox* AddGUIEditBox(Rect2D<int> dimensions, std::wstring text = L"text", glm::vec4 text_color = glm::vec4(1, 1, 1, 1), bool drawbackground = false, bool drawshadow = false, bool clearonsubmit = false);
	GUISlider* AddGUISlider(Rect2D<int> dimensions, float min, float max, float pos, bool drawValue = false, bool snap = false, bool wide = false, bool vertical = false, bool dark = false);
	GUIImage* AddGUIImage(Rect2D<int> dimensions, TexturePtr tex, bool multichannel = true, bool glTex = false);
	GUIWindow* AddGUIWindow(Rect2D<int> dimensions, std::wstring titlebar_text = L"Window", bool clip = true, bool showclose = true, bool modal = false, bool movable = true);
	GUIPane* AddGUIPane(Rect2D<int> dimensions, bool draw = true);
private:
	GUISkin* skin;
	TexturePtr skin_atlas;
	ShaderPtr gui_shader;

	Quad* gui_quad;
	SlicedGUIQuad* sliced_quad;

	FontRenderer* m_font_renderer;
	ApplicationWindow * m_window;

	sigc::connection _sig_mouse_move, _sig_mouse_button, _sig_mouse_scroll, _sig_key, _sig_text;

	GUIElement *hover, *last_hover, *focus, *last_focus, *modal;

	bool m_mouse_down, m_mouse_moved, m_mouse_dragged;

	wchar_t last_char;
	int32_t last_key, last_mod;
	std::wstring clipboard_string;

	glm::vec2 mouse_pos, last_mouse_pos, gui_scale;
protected:
};