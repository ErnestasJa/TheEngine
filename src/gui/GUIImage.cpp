#include "Precomp.h"

#include "opengl/Texture.h"

#include "GUI.h"

GUIImage::GUIImage(GUIEnvironment* env, Rect2D<int> dimensions, std::shared_ptr<Texture> tex, bool multichannel) :GUIElement(env, dimensions)
{
	this->Type = GUIET_ELEMENT;
	environment = env;

	absolute_rect = dimensions;
	relative_rect = absolute_rect;

	m_tex = tex;
	m_multichannel = multichannel;
	_caption = nullptr;
	//it's an image...
	this->SetListening(false);
}

GUIImage::~GUIImage()
{
	if (_caption)
	{
		delete _caption;
	}
}

void GUIImage::SetImage(std::shared_ptr<Texture> tex)
{
	if (tex != nullptr)
		m_tex = tex;
	else
		return;
}

void GUIImage::SetCaption(const std::wstring& caption)
{
	if (!_caption)
	{
		_caption=environment->AddGUIStaticText(Rect2D<int>(0, 0, absolute_rect.w, 24), caption, false);
		_caption->SetParent(this);
	}
	else
	{
		_caption->SetText(caption);
	}
}

void GUIImage::Render()
{
	environment->DrawGUIQuad(absolute_rect, m_tex, false, m_multichannel);

	this->RenderChildren();
}