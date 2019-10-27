#include "Precomp.h"

#include "gui/GUIEnvironment.h"
#include "gui/GUIElement.h"

static uint32_t _elementIdCounter = 0;

GUIElement::GUIElement(GUIEnvironment *env, Rect2D<int> dimensions)
{
    this->Type = GUIET_ELEMENT;
    absolute_rect = dimensions;
    relative_rect = dimensions;

    Id = -1;

    _hAlign = GUIElementHAlignment::HALIGN_LEFT;
    _vAlign = GUIElementVAlignment::VALIGN_TOP;

    hovered = false;
    visible = true;
    focused = false;
    enabled = true;
    modal = false;
    accept_events = true;

    this->parent = nullptr;
    this->event_listener = nullptr;
    this->environment = env;
    SetName(std::string("gui_element_") + helpers::to_str(_elementIdCounter++));
}

GUIElement::~GUIElement()
{
    if (parent) {
        parent->RemoveChild(this);
    }
    DestroyChildren();
}

void GUIElement::SetId(uint32_t Id)
{
    this->Id = Id;
}

uint32_t GUIElement::GetId()
{
    return this->Id;
}

void GUIElement::DestroyChildren()
{
    while (!children.empty()) {
        delete children[0];
    }
    children.clear();
}

void GUIElement::Update(float dt)
{
    // will be overriden by everything
}

void GUIElement::Render()
{
    // will be overriden by everything
}

void GUIElement::AddChild(GUIElement *child)
{
    vector<GUIElement *>::iterator i = std::find(children.begin(), children.end(), child);
    if (i != children.end()) return;

    child->parent = this;
    children.push_back(child);

    child->relative_rect = child->absolute_rect;
    UpdateAbsolutePos();
}

void GUIElement::RemoveChild(GUIElement *child)
{
    vector<GUIElement *>::iterator i = std::find(children.begin(), children.end(), child);
    if (i != children.end()) {
        (*i)->relative_rect = (*i)->absolute_rect;
        (*i)->parent = nullptr;
        children.erase(i);
    }
}

void GUIElement::BringToFront(GUIElement *e)
{
    vector<GUIElement *>::iterator i = std::find(children.begin(), children.end(), e);
    if (i != children.end()) {
        i = children.erase(i);
        children.push_back(e);
        return;
    }
}

void GUIElement::SendToBack(GUIElement* e)
{
	vector<GUIElement*>::iterator i = std::find(children.begin(), children.end(), e);
	if (i != children.end()) {
		i = children.erase(i);
		children.insert(children.begin(),e);
		return;
	}
}

void GUIElement::UpdateChildren(float dt)
{
    Update(dt);

    for (GUIElement *i : children) {
        if (i->IsVisible()) i->UpdateChildren(dt);
    }
}

void GUIElement::RenderChildren()
{
    for (GUIElement *i : children) {
        if (i->IsVisible()) i->Render();
    }
}

void GUIElement::UpdateAbsolutePos()
{
    if (this->parent != nullptr) {
        this->absolute_rect.x = relative_rect.x + parent->absolute_rect.x;
        this->absolute_rect.y = relative_rect.y + parent->absolute_rect.y;

        this->absolute_rect.w = relative_rect.w;
        this->absolute_rect.h = relative_rect.h;

        this->absolute_rect.calculate_bounds();
    }

    for (GUIElement *e : children) e->UpdateAbsolutePos();
}

void GUIElement::Move(const glm::vec2 &pos)
{
    relative_rect.SetPosition(pos.x, pos.y);
    UpdateAbsolutePos();
}

void GUIElement::SetAlignment(GUIElementHAlignment hAlign, GUIElementVAlignment vAlign)
{
    _hAlign = hAlign;
    _vAlign = vAlign;

    if (this->parent != nullptr) {
        switch (_hAlign) {
            case HALIGN_CENTER: {
                this->relative_rect.x = parent->absolute_rect.w / 2 - relative_rect.w / 2 + relative_rect.x;
                break;
            }

            case HALIGN_RIGHT: {
                this->relative_rect.x = parent->absolute_rect.w - relative_rect.w - relative_rect.x;
                break;
            }
        }

        switch (_vAlign) {
            case VALIGN_CENTER: {
                this->relative_rect.y = parent->absolute_rect.h / 2 - relative_rect.h / 2 - relative_rect.y;
                break;
            }

            case VALIGN_BOTTOM: {
                this->relative_rect.y = parent->absolute_rect.h - relative_rect.h - relative_rect.y;
                break;
            }
        }

        this->relative_rect.calculate_bounds();

        UpdateAbsolutePos();
    }
}

void GUIElement::SetName(std::string name)
{
    this->name = name;
}

void GUIElement::SetEventListener(GUIEventListener *listener)
{
    this->event_listener = listener;
}

GUIEventListener *GUIElement::GetEventListener()
{
    return this->event_listener;
}

bool GUIElement::OnEvent(const GUIEvent &e)
{
    if (this->event_listener && this->event_listener->OnEvent(e)) return true;

    return parent ? parent->OnEvent(e) : false;
}

void GUIElement::SetEnabled(bool b)
{
    this->enabled = b;
    if (children.size() > 0)
        for (GUIElement *e : children) e->SetEnabled(b);
}

void GUIElement::SetFocused(bool b)
{
    this->focused = b;
}

void GUIElement::SetVisible(bool b)
{
    if (this->IsHovered()) {
        this->SetHovered(false);
        environment->SetHoverElement(nullptr);
    }

    if (this->IsFocused()) {
        this->SetFocused(false);
        environment->SetFocusElement(nullptr);
    }

    if (modal) {
        if (b) {
            environment->SetModal(this);
        } else {
            environment->RemoveModal();
        }
    }

    this->visible = b;

    for (GUIElement* e : children)
        e->SetVisible(b);
}

void GUIElement::SetHovered(bool b)
{
    this->hovered = b;
}

void GUIElement::SetListening(bool b)
{
    this->accept_events = b;
}

void GUIElement::SetParent(GUIElement *newparent)
{
    if (newparent) {
        if (this->parent) this->parent->RemoveChild(this);
        newparent->AddChild(this);
    } else
        return;
}

bool GUIElement::IsEnabled()
{
    return this->enabled;
}

bool GUIElement::IsFocused()
{
    return this->focused;
}

bool GUIElement::IsVisible()
{
    return this->visible;
}

bool GUIElement::IsHovered()
{
    return this->hovered;
}

bool GUIElement::IsModal()
{
    return this->modal;
}

bool GUIElement::AcceptsEvents()
{
    return this->accept_events;
}

GUIEnvironment *GUIElement::GetEnvironment()
{
    return this->environment;
}

GUIElement *GUIElement::GetParent()
{
    return this->parent;
}

vector<GUIElement *> &GUIElement::GetChildren()
{
    return this->children;
}

std::string GUIElement::GetName()
{
    return this->name;
}

GUIElement *GUIElement::GetElementFromPoint(int x, int y)
{
    GUIElement *ret = nullptr;
    vector<GUIElement *>::reverse_iterator i = children.rbegin();
    for (; i != children.rend(); i++) {
        if (*i != nullptr) {
            ret = (*i)->GetElementFromPoint(x, y);
            if (ret != nullptr && ret->AcceptsEvents()) return ret;
        }
    }
    if (this->IsVisible() && absolute_rect.is_point_inside(x, y)) ret = this;
    return ret;
}

Rect2D<int> &GUIElement::GetAbsoluteRect()
{
    return absolute_rect;
}

Rect2D<int> &GUIElement::GetRelativeRect()
{
    return relative_rect;
}

GUIElement *GUIElement::search_elements(GUIElement *el, const std::string &name)
{
    if (el->GetName() == name) {
        return el;
    }

    loop(i, el->GetChildren().size())
    {
        GUIElement *e = search_elements(el->GetChildren()[i], name);

        if (e) {
            return e;
        }
    }

    return nullptr;
}

GUIElement *GUIElement::get_element_by_name(const std::string &name)
{
    return search_elements(this, name);
}