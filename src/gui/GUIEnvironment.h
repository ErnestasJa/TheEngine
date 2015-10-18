#pragma once

#include "GUIElement.h"
#include "GUISkin.h"
#include "font_rendering/FontRenderer.h"

class Shader;
class GUIQuad;
class SlicedGUIQuad;
class Texture;
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

	void update(float delta);
	void Render();

	virtual bool OnEvent(const GUIEvent & e);
	bool is_on_hover(GUIElement *e);
	bool is_on_focus(GUIElement *e);

	void draw_gui_quad(Rect2D<int> size, std::shared_ptr<Texture> tex, bool tile = false, bool multichannel = true);
	void draw_gui_quad(Rect2D<int> size, uint32_t style = gui_style::gui_skin_background, bool tile = false);
	void draw_gui_quad(Rect2D<int> size, glm::vec4 col = glm::vec4(1.f));

	void draw_sliced_gui_quad(Rect2D<int> size, std::shared_ptr<Texture> tex, bool tile = false);
	void draw_sliced_gui_quad(Rect2D<int> size, uint32_t style = gui_style::gui_skin_background);
	void draw_sliced_gui_quad(Rect2D<int> size, glm::vec4 col = glm::vec4(1.f));

	void set_skin(GUISkin* skin);

	glm::vec2 get_mouse_pos();
	glm::vec2 get_gui_scale();

	void on_mouse_moved(double x, double y);
	void on_mouse_button(int32_t button, int32_t action, int32_t mod);
	void on_mouse_scroll(double sx, double sy);
	void on_key_event(int32_t key, int32_t scan_code, int32_t action, int32_t mod);
	void on_char_typed(int32_t scan_code);

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
		T gsx = get_gui_scale().x;
		T gsy = get_gui_scale().y;
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
	GUISlider* AddGUISlider(Rect2D<int> dimensions, float min, float max, float pos, bool vertical = false);
	GUIImage* AddGUIImage(Rect2D<int> dimensions, std::shared_ptr<Texture> tex, bool multichannel = true);
	GUIWindow* AddGUIWindow(Rect2D<int> dimensions, std::wstring titlebar_text = L"Window", bool clip = true, bool showclose = true, bool modal = false, bool movable = true);
	GUIPane* AddGUIPane(Rect2D<int> dimensions, bool draw = true);
private:
	GUISkin* skin;
	Texture* skin_atlas;
	Shader* gui_shader;

	GUIQuad* gui_quad;
	SlicedGUIQuad* sliced_quad;

	FontRenderer* m_font_renderer;
	ApplicationWindow * m_window;

	sigc::connection _sig_mouse_move, _sig_mouse_button, _sig_mouse_scroll, _sig_key, _sig_text;

	GUIElement *hover, *last_hover, *focus, *last_focus;

	bool m_mouse_down, m_mouse_moved, m_mouse_dragged;

	wchar_t last_char;
	int32_t last_key, last_mod;
	std::wstring clipboard_string;

	glm::vec2 mouse_pos, last_mouse_pos, gui_scale;
protected:
};