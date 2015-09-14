#include "Precomp.h"

#include "GUI/GUIButton.h"

#include "ToggleGroup.h"

ToggleGroup::ToggleGroup(bool multitoggle)
{
	this->_multiToggle = multitoggle;
}

ToggleGroup::ToggleGroup(vector<GUIButton*> predef, bool multitoggle)
{
	this->_multiToggle = multitoggle;

	for (auto &button : predef)
	{
		AddButton(button);
	}
}

void ToggleGroup::AddButton(GUIButton* btn)
{
	_elements[btn->GetName()] = btn;
}

GUIButton* ToggleGroup::GetToggledButton()
{
	return _elements[_currentToggle];
}

void ToggleGroup::SetToggledButton(const std::string &elem)
{
	if (Toggle(_elements[elem]))
	{
		_currentToggle = elem;
	}
}

bool ToggleGroup::Toggle(GUIButton* btn)
{
	bool success = false;
	for (auto &el : _elements)
	{
		if (this->_multiToggle)
		{
			if (el.first == btn->GetName())
			{
				el.second->SetToggled(!el.second->IsToggled());
				return true;
			}
		}
		else
		{
			if (el.first.compare(btn->GetName()) == 0)
			{
				el.second->SetToggled(true);
				success = true;
			}
			else
				el.second->SetToggled(false);
		}
	}
	return success;
}

bool ToggleGroup::ContainsButton(const std::string &elem)
{
	return _elements.count(elem) != 0;
}