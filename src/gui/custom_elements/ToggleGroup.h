#pragma once

class GUIButton;

class ToggleGroup
{
private:
	std::map<std::string, GUIButton*> _elements;
	bool _multiToggle;
	std::string _currentToggle;

	bool Toggle(GUIButton* btn);
public:
	ToggleGroup(bool multitoggle = false);

	ToggleGroup(vector<GUIButton*> predef, bool multitoggle = false);

	void AddButton(GUIButton* btn);

	GUIButton* GetToggledButton();

	void SetToggledButton(const std::string &elem);

	bool ContainsButton(const std::string &elem);
};
