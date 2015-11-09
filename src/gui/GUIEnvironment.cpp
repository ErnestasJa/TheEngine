#include "Precomp.h"
#include "GUI.h"
#include "opengl/material/Shader.h"
#include "opengl/geometry/Quad.h"
#include "opengl/geometry/SlicedGUIQuad.h"
#include "GUISkin.h"
#include "resources/Image.h"
#include "resources/ImageLoader.h"
#include "resources/ResourceManager.h"
#include "opengl/Texture.h"
#include "application/Window.h"
#include "application/AppContext.h"

GUIEnvironment::GUIEnvironment() :GUIElement(nullptr, Rect2D<int>(0, 0, GetContext().GetWindow()->GetWindowSize().x, GetContext().GetWindow()->GetWindowSize().y))
{
	GetContext().SetGUIEnvironment(this);

	this->m_window = GetContext().GetWindow();

	_sig_mouse_move = m_window->SigMouseMoved().connect(sigc::mem_fun(this, &GUIEnvironment::OnMouseMove));
	_sig_mouse_button = m_window->SigMouseKey().connect(sigc::mem_fun(this, &GUIEnvironment::OnMouseClick));
	_sig_mouse_scroll = m_window->SigMouseScroll().connect(sigc::mem_fun(this, &GUIEnvironment::OnMouseScroll));
	_sig_key = m_window->SigKeyEvent().connect(sigc::mem_fun(this, &GUIEnvironment::OnKeyEvent));
	_sig_text = m_window->SigTextEvent().connect(sigc::mem_fun(this, &GUIEnvironment::OnCharacterTyped));

	glm::ivec2 win_dims = m_window->GetWindowSize();
	this->disp_w = win_dims.x;
	this->disp_h = win_dims.y;

	hover = last_hover = focus = last_focus = modal = nullptr;

	m_mouse_down = m_mouse_dragged = m_mouse_moved = false;

	mouse_pos = last_mouse_pos = glm::vec2();

	gui_scale = glm::vec2(2.0 / (float)disp_w, 2.0 / (float)disp_h);

	this->SetName("GUI_ENVIRONMENT");
	last_char = ' ';

	gui_shader = GetContext().GetResourceManager()->LoadShader("res/engine/shaders/gui_quad");
	gui_quad = new Quad();

	sliced_quad = new SlicedGUIQuad(glm::vec2(1), 8);
	sliced_quad->Init();

	skin = new GUISkin();

	skin->load("res/gui/skins/skin_default.xml");

	skin_atlas = share(new Texture());
	ImageLoader * imgl = new ImageLoader();
	auto img = std::shared_ptr<Image>(imgl->Load("res/gui/skins/skin_default2.png"));
	skin_atlas->Init(img);
	delete imgl;

	m_font_renderer = new FontRenderer();
}

GUIEnvironment::~GUIEnvironment()
{
	_sig_mouse_move.disconnect();
	_sig_mouse_button.disconnect();
	_sig_mouse_scroll.disconnect();
	_sig_key.disconnect();
	_sig_text.disconnect();
	delete m_font_renderer;
	gui_shader = nullptr;
	delete gui_quad;
	delete sliced_quad;
}

GUIStaticText* GUIEnvironment::AddGUIStaticText(Rect2D<int> dimensions, std::wstring text, bool drawbackground)
{
	auto ret = new GUIStaticText(this, dimensions, text, drawbackground);
	ret->SetParent(this);
	return ret;
}

GUIButton*  GUIEnvironment::AddGUIButton(Rect2D<int> dimensions, std::wstring text, bool colored, bool toggling, bool toggleStatus)
{
	auto ret = new GUIButton(this, dimensions, text, colored, toggling, toggleStatus);
	ret->SetParent(this);
	return ret;
}

GUICheckbox* GUIEnvironment::AddGUICheckbox(Rect2D<int> dimensions, bool checked)
{
	auto ret = new GUICheckbox(this, dimensions, checked);
	ret->SetParent(this);
	return ret;
}

GUIEditBox* GUIEnvironment::AddGUIEditBox(Rect2D<int> dimensions, std::wstring text, glm::vec4 text_color, bool drawbackground, bool drawshadow, bool clearonsubmit)
{
	auto ret = new GUIEditBox(this, dimensions, text, text_color, drawbackground, drawshadow, clearonsubmit);
	ret->SetParent(this);
	return ret;
}

GUISlider* GUIEnvironment::AddGUISlider(Rect2D<int> dimensions, float min, float max, float pos, bool vertical)
{
	auto ret = new GUISlider(this, dimensions, min, max, pos, vertical);
	ret->SetParent(this);
	return ret;
}

GUIImage* GUIEnvironment::AddGUIImage(Rect2D<int> dimensions, TexturePtr tex, bool multichannel)
{
	auto ret = new GUIImage(this, dimensions, tex, multichannel);
	ret->SetParent(this);
	return ret;
}

GUIWindow* GUIEnvironment::AddGUIWindow(Rect2D<int> dimensions, std::wstring titlebar_text, bool clip, bool showclose, bool modal, bool movable)
{
	auto ret = new GUIWindow(this, dimensions, titlebar_text, clip, showclose, modal, movable);
	ret->SetParent(this);
	return ret;
}

GUIPane* GUIEnvironment::AddGUIPane(Rect2D<int> dimensions, bool draw)
{
	auto ret = new GUIPane(this, dimensions, draw);
	ret->SetParent(this);
	return ret;
}

void GUIEnvironment::update(float delta)
{
}

void GUIEnvironment::Render()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	this->RenderChildren();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

bool GUIEnvironment::OnEvent(const GUIEvent & e)
{
	GUI_BEGIN_ON_EVENT(e)

		switch (e.GetType())
		{
		case element_focused:
			this->BringToFront(e.get_caller());
			break;
		default:
			break;
		}

	GUI_END_ON_EVENT(e)
}

void GUIEnvironment::OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t mod)
{
	//printf("Key event: Key:%i SC:%i Action:%i Mod:%i\n",key,scan_code,action,mod);

	this->last_key = key;

	if (focus != nullptr)
	{
		switch (action)
		{
		case GLFW_PRESS:
			switch (mod)
			{
			case GLFW_MOD_CONTROL:
				switch (key)
				{
				case GLFW_KEY_A:
					break;
				case GLFW_KEY_X:
					break;
				case GLFW_KEY_C:
					break;
				case GLFW_KEY_V:
					this->clipboard_string = helpers::to_wstr(glfwGetClipboardString(this->m_window->GetWindow()));
					focus->OnEvent(GUIEvent(text_paste, this, focus));
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			focus->OnEvent(GUIEvent(key_pressed, this, focus));
			break;
		case GLFW_REPEAT:
			focus->OnEvent(GUIEvent(key_pressed, this, focus));
			break;
		case GLFW_RELEASE:
			break;

		default:
			break;
		}
	}
}

void GUIEnvironment::OnCharacterTyped(int32_t scan_code)
{
	this->last_char = (wchar_t)scan_code;
	if (focus != nullptr)
		focus->OnEvent(GUIEvent(key_typed, this, focus));
}

void GUIEnvironment::OnMouseMove(double x, double y)
{
	mouse_pos = glm::vec2(x, y);

	GUIElement *target = target = GetElementFromPoint(mouse_pos.x, mouse_pos.y);

	//printf("Target name: %s @ %f %f\n", target != nullptr ? target->GetName().c_str() : "das is null", mouse_pos.x, mouse_pos.y);

	//only update elements which are enabled,visible and accept events
	if (target != nullptr)
	{
		if (target->IsEnabled() && target->IsVisible() && target->AcceptsEvents())
		{
			if (target != hover)
			{
				if (hover != nullptr)
				{
					last_hover = hover;
					last_hover->OnEvent(GUIEvent(
						gui_event_type::element_exitted, this,
						last_hover));
					last_hover->SetHovered(false);
				}
				hover = target;
				hover->OnEvent(GUIEvent(
					gui_event_type::element_hovered, this, hover));
				hover->SetHovered(true);
			}

			if (focus != nullptr)
				if (m_mouse_down)
					focus->OnEvent(GUIEvent(mouse_dragged, this, focus));
		}
	}
}

void GUIEnvironment::OnMouseClick(int32_t button, int32_t action, int32_t mod)
{
	switch (action)
	{
	case GLFW_PRESS:
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			m_mouse_down = true;
		}

		//focusing
		if (hover != focus)
		{
			if (focus != nullptr)
			{
				last_focus = focus;
				GUI_FIRE_ELEMENT_EVENT(last_focus, GUIEvent(gui_event_type::element_focus_lost, this, last_focus))
					last_focus->SetFocused(false);
			}
			focus = hover;
			if (hover != this)
			{
				GUI_FIRE_ELEMENT_EVENT(focus, GUIEvent(gui_event_type::element_focused, this, focus))
					focus->SetFocused(true);
				focus->GetParent()->BringToFront(focus);
			}
			else
				focus = nullptr;
		}

		if (focus != nullptr&&hover == focus)
		{
			gui_event_type eventType = button == GLFW_MOUSE_BUTTON_LEFT ? left_mouse_pressed : button == GLFW_MOUSE_BUTTON_RIGHT ? right_mouse_pressed : middle_mouse_pressed;
			focus->OnEvent(GUIEvent(eventType, this, focus));
		}
		break;
	case GLFW_RELEASE:
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			m_mouse_down = false;
		}

		if (focus != nullptr&&hover == focus)
		{
			gui_event_type eventType = button == GLFW_MOUSE_BUTTON_LEFT ? left_mouse_released : button == GLFW_MOUSE_BUTTON_RIGHT ? right_mouse_released : middle_mouse_released;
			focus->OnEvent(GUIEvent(eventType, this, focus));
		}
		break;
	default:break;
	}
}

void GUIEnvironment::OnMouseScroll(double sx, double sy)
{
	return;
}

glm::vec2 GUIEnvironment::GetMousePosition()
{
	return mouse_pos;
}

glm::vec2 GUIEnvironment::GetGUIScaling()
{
	return gui_scale;
}

FontRenderer* GUIEnvironment::GetFontRenderer()
{
	return m_font_renderer;
}

void GUIEnvironment::DrawGUIQuad(Rect2D<int> dims, TexturePtr tex, bool tile, bool multichannel)
{
	Rect2D<float> scaled_dims = ScaleGUIRect(dims.as<float>());

	tex->Set(0);

	gui_quad->SetUV(skin->get_uv(gui_skin_whole_texture));

	glm::mat4 M = glm::mat4(1.0f);

	M = glm::translate(M, glm::vec3(scaled_dims.x, scaled_dims.y, 0));
	M = glm::scale(M, glm::vec3(scaled_dims.w, scaled_dims.h, 0));

	SetBindingSafe(gui_shader, "tex", 0);
	SetBindingSafe(gui_shader, "M", M);
	SetBindingSafe(gui_shader, "singlechannel", !multichannel);
	SetBindingSafe(gui_shader, "colored", GL_FALSE);
	SetBindingSafe(gui_shader, "color", glm::vec4(0.f));
	SetBindingSafe(gui_shader, "alpha", 1.0f);

	gui_shader->Set();
	gui_quad->Render();

	tex->Unset(0);
}

void GUIEnvironment::DrawGUIQuad(Rect2D<int> dims, uint32_t style, bool tile)
{
	Rect2D<float> scaled_dims = ScaleGUIRect(dims.as<float>());

	skin_atlas->Set(0);

	gui_quad->SetUV(skin->get_uv(style));

	glm::mat4 M = glm::mat4(1.0f);

	M = glm::translate(M, glm::vec3(scaled_dims.x, scaled_dims.y, 0));
	M = glm::scale(M, glm::vec3(scaled_dims.w, scaled_dims.h, 0));

	SetBindingSafe(gui_shader, "tex", 0);
	SetBindingSafe(gui_shader, "M", M);
	SetBindingSafe(gui_shader, "singlechannel", GL_FALSE);
	SetBindingSafe(gui_shader, "colored", GL_FALSE);
	SetBindingSafe(gui_shader, "color", glm::vec4(0.f));
	SetBindingSafe(gui_shader, "alpha", 1.0f);

	gui_shader->Set();
	gui_quad->Render();

	skin_atlas->Unset(0);
}

void GUIEnvironment::DrawGUIQuad(Rect2D<int> dims, glm::vec4 col)
{
	Rect2D<float> scaled_dims = ScaleGUIRect(dims.as<float>());

	glm::mat4 M = glm::mat4(1.0f);

	M = glm::translate(M, glm::vec3(scaled_dims.x, scaled_dims.y, 0));
	M = glm::scale(M, glm::vec3(scaled_dims.w, scaled_dims.h, 0));

	SetBindingSafe(gui_shader, "tex", 0);
	SetBindingSafe(gui_shader, "M", M);
	SetBindingSafe(gui_shader, "singlechannel", GL_FALSE);
	SetBindingSafe(gui_shader, "colored", GL_TRUE);
	SetBindingSafe(gui_shader, "color", col);
	SetBindingSafe(gui_shader, "alpha", 1.0f);

	gui_shader->Set();
	gui_quad->Render();
}

void GUIEnvironment::DrawSlicedGUIQuad(Rect2D<int> size, TexturePtr tex, bool tile)
{
	Rect2D<float> scaled_dims = ScaleGUIRect(size.as<float>());

	tex->Set(0);

	glm::mat4 M = glm::mat4(1.0f);

	M = glm::translate(M, glm::vec3(scaled_dims.x, scaled_dims.y, 0));
	M = glm::scale(M, glm::vec3(scaled_dims.w, scaled_dims.h, 0));

	SetBindingSafe(gui_shader, "tex", 0);
	SetBindingSafe(gui_shader, "M", M);
	SetBindingSafe(gui_shader, "singlechannel", GL_FALSE);
	SetBindingSafe(gui_shader, "colored", GL_FALSE);
	SetBindingSafe(gui_shader, "color", glm::vec4(0.f));
	SetBindingSafe(gui_shader, "alpha", 1.0f);

	sliced_quad->SetRatio(glm::vec2(size.w, size.h));
	gui_shader->Set();
	sliced_quad->Render();

	tex->Unset(0);
}

void GUIEnvironment::DrawSlicedGUIQuad(Rect2D<int> size, uint32_t style)
{
	Rect2D<float> scaled_dims = ScaleGUIRect(size.as<float>());

	skin_atlas->Set(0);

	sliced_quad->SetRatio(glm::vec2(size.w, size.h), skin->get_margin(style));
	sliced_quad->SetTCoords(skin->get_uv(style));

	glm::mat4 M = glm::mat4(1.0f);

	M = glm::translate(M, glm::vec3(scaled_dims.x, scaled_dims.y, 0));
	M = glm::scale(M, glm::vec3(scaled_dims.w, scaled_dims.h, 0));

	SetBindingSafe(gui_shader, "tex", 0);
	SetBindingSafe(gui_shader, "M", M);
	SetBindingSafe(gui_shader, "singlechannel", GL_FALSE);
	SetBindingSafe(gui_shader, "colored", GL_FALSE);
	SetBindingSafe(gui_shader, "color", glm::vec4(0.f));
	SetBindingSafe(gui_shader, "alpha", 0.9f);

	gui_shader->Set();
	sliced_quad->Render();

	skin_atlas->Unset(0);
}

void GUIEnvironment::DrawSlicedGUIQuad(Rect2D<int> size, glm::vec4 col)
{
	Rect2D<float> scaled_dims = ScaleGUIRect(size.as<float>());

	glm::mat4 M = glm::mat4(1.0f);

	M = glm::translate(M, glm::vec3(scaled_dims.x, scaled_dims.y, 0));
	M = glm::scale(M, glm::vec3(scaled_dims.w, scaled_dims.h, 0));

	SetBindingSafe(gui_shader, "tex", 0);
	SetBindingSafe(gui_shader, "M", M);
	SetBindingSafe(gui_shader, "singlechannel", GL_FALSE);
	SetBindingSafe(gui_shader, "colored", GL_TRUE);
	SetBindingSafe(gui_shader, "color", col);
	SetBindingSafe(gui_shader, "alpha", 1.f);

	sliced_quad->SetRatio(glm::vec2(size.w, size.h));
	gui_shader->Set();
	sliced_quad->Render();
}