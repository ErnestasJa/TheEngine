#include "Precomp.h"
#include "gui/GUI.h"
#include "resources/Image.h"
#include "GUIColorPicker.h"
#include "resources/ImageLoader.h"
#include "opengl/Texture.h"
#include "application/AppContext.h"
#include "utility/Helpers.h"

GUIColorPicker::GUIColorPicker(GUIEnvironment* env, Rect2D<int> dimensions, bool drawbackground) :GUIElement(env, dimensions)
{
	this->Type = GUIET_PANE;
	environment = env;

	primaryColor = secondaryColor = glm::vec4(255, 255, 255, 255); //white

	colHSV = glm::vec3(0, 0, 1); //white

	cursorPos = glm::vec2(0, 0);

	if (drawbackground)
	{
		bg = new GUIPane(env, Rect2D<int>(0, 0, dimensions.w + 64 + 16, dimensions.h + 32 + 20));
		bg->SetParent(this);
		bg->SetListening(false);
	}

	imgBuf = share(new Image());
	imgBuf->Init(dimensions.w, dimensions.h, 3);
	texBuf = share(new Texture());
	texBuf->Init(imgBuf);

	picker = new GUIImage(env, Rect2D<int>(16, 16, dimensions.w, dimensions.h), texBuf);
	picker->SetParent(this);
	picker->SetListening(true);

	ImageLoader* ldr = new ImageLoader();
	TexturePtr cursorTex = share(new Texture());
	cursorTex->Init(ldr->Load("res/gui/images/cpcurs.png"));
	cursor = new GUIImage(env, Rect2D<int>(cursorPos.x, cursorPos.y, 8, 8), cursorTex);
	cursor->SetParent(picker);
	cursor->SetListening(false);

	sat = new GUISlider(env, Rect2D<int>(16, dimensions.h + 16 + 4, dimensions.w, 8), 0, 1, 1, false);
	sat->SetParent(this);

	stR = new GUIStaticText(env, Rect2D<int>(picker->GetRelativeRect().w + 4, picker->GetRelativeRect().y + 36, 16, 16), L"['s]R:[s']");
	stR->SetParent(picker);

	stG = new GUIStaticText(env, Rect2D<int>(picker->GetRelativeRect().w + 4, picker->GetRelativeRect().y + 60, 16, 16), L"['s]G:[s']");
	stG->SetParent(picker);

	stB = new GUIStaticText(env, Rect2D<int>(picker->GetRelativeRect().w + 4, picker->GetRelativeRect().y + 84, 16, 16), L"['s]B:[s']");
	stB->SetParent(picker);

	stA = new GUIStaticText(env, Rect2D<int>(picker->GetRelativeRect().w + 4, picker->GetRelativeRect().y + 108, 16, 16), L"['s]A:[s']");
	stA->SetParent(picker);

	ebR = new GUIEditBox(env, Rect2D<int>(picker->GetRelativeRect().w + 16, picker->GetRelativeRect().y + 32, 32, 16), L"255", glm::vec4(1, 0, 0, 1));
	ebR->SetParent(picker);
	ebR->SetMaxLength(3);

	ebG = new GUIEditBox(env, Rect2D<int>(picker->GetRelativeRect().w + 16, picker->GetRelativeRect().y + 56, 32, 16), L"255", glm::vec4(0, 1, 0, 1));
	ebG->SetParent(picker);
	ebG->SetMaxLength(3);

	ebB = new GUIEditBox(env, Rect2D<int>(picker->GetRelativeRect().w + 16, picker->GetRelativeRect().y + 80, 32, 16), L"255", glm::vec4(0, 0, 1, 1));
	ebB->SetParent(picker);
	ebB->SetMaxLength(3);

	ebA = new GUIEditBox(env, Rect2D<int>(picker->GetRelativeRect().w + 16, picker->GetRelativeRect().y + 104, 32, 16), L"255", glm::vec4(1, 1, 1, 1));
	ebA->SetParent(picker);
	ebA->SetMaxLength(3);

	stHTML = new GUIStaticText(env, Rect2D<int>(4, dimensions.h + 36, 64, 16), L"['s]HTML Hex:[s']");
	stHTML->SetParent(this);

	ebHTML = new GUIEditBox(env, Rect2D<int>(64, dimensions.h + 32, 128, 16), L"", glm::vec4(1, 1, 1, 1));
	ebHTML->SetParent(this);
	ebHTML->SetMaxLength(8);

	btnSwitchColor = new GUIButton(env, Rect2D<int>(picker->GetRelativeRect().w + 16, picker->GetRelativeRect().y + 16, 32, 12), L"['s]<->[s']");
	btnSwitchColor->SetParent(picker);

	GenerateHSVMap(sat->get_value());
	UpdateValues();
	secondaryColor = primaryColor;
	UpdateAbsolutePos();
	this->SetListening(false);
}

GUIColorPicker::~GUIColorPicker()
{
	//dtor
}

void GUIColorPicker::GenerateHSVMap(float s)
{
	loop(x, imgBuf->width)
	{
		loop(y, imgBuf->height)
		{
			glm::vec3 co = helpers::hsv2rgb(360.f / imgBuf->width * x, s, 1.f / imgBuf->height*y);
			imgBuf->SetPixel(x, y, 255.f*co.x, 255.f*co.y, 255.f*co.z);
		}
	}
	texBuf->SetSubImage2D(imgBuf->data, 0, 0, imgBuf->width, imgBuf->height);
}

bool GUIColorPicker::OnEvent(const GUIEvent &e)
{
	GUI_BEGIN_ON_EVENT(e)

		switch (e.GetType())
		{
		case left_mouse_pressed:
		case mouse_dragged:
			if (e.get_element() == picker)
			{
				float mx = environment->GetMousePosition().x;
				float my = environment->GetMousePosition().y;
				if (picker->GetAbsoluteRect().is_point_inside(mx, my))
				{
					float x = mx - absolute_rect.x - picker->GetRelativeRect().x - 4;
					float y = my - absolute_rect.y - picker->GetRelativeRect().y - 4;
					cursorPos = glm::vec2(x, y);
					cursor->Move(cursorPos);
					//                cursor->GetRelativeRect().clip(picker->GetAbsoluteRect());
					//                cursor->UpdateAbsolutePos();

					BringToFront(cursor);

					UpdateValues();
				}
			}
			break;
		case key_typed:
		{
			if (e.get_element() == ebR || e.get_element() == ebG || e.get_element() == ebB || e.get_element() == ebA)
			{
				auto primary = primaryColor;

				uint32_t r = primaryColor.r;
				uint32_t g = primaryColor.g;
				uint32_t b = primaryColor.b;
				uint32_t a = primaryColor.a;

				if (ebR->get_text().length() > 0)
				{
					r = helpers::wtoi(ebR->get_text().c_str());
				}
				if (ebG->get_text().length() > 0)
				{
					g = helpers::wtoi(ebG->get_text().c_str());
				}
				if (ebB->get_text().length() > 0)
				{
					b = helpers::wtoi(ebB->get_text().c_str());
				}
				if (ebA->get_text().length() > 0)
				{
					a = helpers::wtoi(ebA->get_text().c_str());
				}

				primaryColor = glm::vec4(r, g, b, a);
				UpdateHTMLColorTextValues();
			}
			if (e.get_element() == ebHTML)
			{
				auto text = ebHTML->get_text();
				if (text.length() >= 6)
				{
					uint32_t r = 0;
					uint32_t g = 0;
					uint32_t b = 0;
					uint32_t a = 255;

					if (text.length() == 8)
					{
						std::wistringstream(text.substr(6, 2)) >> std::hex >> a;
					}

					std::wistringstream(text.substr(0, 2)) >> std::hex >> r;
					std::wistringstream(text.substr(2, 2)) >> std::hex >> g;
					std::wistringstream(text.substr(4, 2)) >> std::hex >> b;

					primaryColor = glm::vec4(r, g, b, a);
					UpdateColorTextValues();
				}
			}
			break;
		}
		case button_released:
		{
			if (e.get_element() == btnSwitchColor)
			{
				glm::vec4 col = primaryColor;
				primaryColor = secondaryColor;
				secondaryColor = col;
			}
		}
		break;
		case scrollbar_changed:
			GenerateHSVMap(sat->get_value());
			UpdateValues();
			break;
		default:
			break;
		}

	GUI_END_ON_EVENT(e)
}

void GUIColorPicker::UpdateValues()
{
	primaryColor = imgBuf->GetPixel(cursorPos.x + 4, imgBuf->height - 1 - cursorPos.y - 4);
	UpdateColorTextValues();
	UpdateHTMLColorTextValues();
}

void GUIColorPicker::UpdateColorTextValues()
{
	ebR->SetText(helpers::to_wstr(primaryColor.r));
	ebG->SetText(helpers::to_wstr(primaryColor.g));
	ebB->SetText(helpers::to_wstr(primaryColor.b));
	ebA->SetText(helpers::to_wstr(primaryColor.a));
}

unsigned char hexval(unsigned char c)
{
	if ('0' <= c && c <= '9')
		return c - '0';
	else if ('a' <= c && c <= 'f')
		return c - 'a' + 10;
	else if ('A' <= c && c <= 'F')
		return c - 'A' + 10;
	else abort();
}

void hex2ascii(const std::wstring& in, std::wstring& out)
{
	out.clear();
	out.reserve(in.length() / 2);
	for (std::wstring::const_iterator p = in.begin(); p != in.end(); p++)
	{
		unsigned char c = hexval(*p);
		p++;
		if (p == in.end()) break; // incomplete last digit - should report error
		c = (c << 4) + hexval(*p); // + takes precedence over <<
		out.push_back(c);
	}
}

std::wstring toHex(uint8_t value)
{
	auto n = glm::max<int>(0, glm::min<int>(value, 255));

	std::wstring hexstring = L"0123456789ABCDEF";
	auto char1 = hexstring.at((n - n % 16) / 16);
	auto char2 = hexstring.at(n % 16);

	std::wstring resultString = L"";
	resultString += char1;
	resultString += char2;

	return resultString;
}

void GUIColorPicker::UpdateHTMLColorTextValues()
{
	std::wstring val = L"";
	val += toHex(primaryColor.r);
	val += toHex(primaryColor.g);
	val += toHex(primaryColor.b);
	val += toHex(primaryColor.a);
	ebHTML->SetText(val);
}

glm::vec4 GUIColorPicker::GetPrimaryColor()
{
	return primaryColor;
}

glm::vec4 GUIColorPicker::GetPrimaryColorGL()
{
	return 1.f / 255.f*primaryColor;
}

glm::vec4 GUIColorPicker::GetSecondaryColor()
{
	return secondaryColor;
}

glm::vec4 GUIColorPicker::GetSecondaryColorGL()
{
	return 1.f / 255.f*secondaryColor;
}

void GUIColorPicker::SetPrimaryColorRGB(uint8_t r, uint8_t g, uint8_t b)
{
	primaryColor = glm::vec4(r, g, b, 255);
	UpdateColorTextValues();
	UpdateHTMLColorTextValues();
}

void GUIColorPicker::SetPrimaryColorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	primaryColor = glm::vec4(r, g, b, a);
	UpdateColorTextValues();
	UpdateHTMLColorTextValues();
}

void GUIColorPicker::SetSecondaryColorRGB(uint8_t r, uint8_t g, uint8_t b)
{
	secondaryColor = glm::vec4(r, g, b, 255);
}

void GUIColorPicker::SetSecondaryColorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	secondaryColor = glm::vec4(r, g, b, a);
}

void GUIColorPicker::Render()
{
	RenderChildren();
	environment->DrawSlicedGUIQuad(Rect2D<int>(absolute_rect.x + absolute_rect.w + 24 + 4, absolute_rect.y + 16 + 4, 24, 24), helpers::color255(secondaryColor.r, secondaryColor.g, secondaryColor.b, secondaryColor.a));
	environment->DrawSlicedGUIQuad(Rect2D<int>(absolute_rect.x + absolute_rect.w + 24, absolute_rect.y + 16, 24, 24), helpers::color255(primaryColor.r, primaryColor.g, primaryColor.b, primaryColor.a));
}