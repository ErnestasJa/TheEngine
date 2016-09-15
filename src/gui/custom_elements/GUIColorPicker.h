#ifndef GUICOLORPICKER_H
#define GUICOLORPICKER_H

#include "ForwardDecl.h"
#include "gui/GUIElement.h"

template <typename T>
class Rect2D;

class GUIStaticText;
class GUIImage;
class GUIEditBox;
class GUIButton;
class GUIPane;
class GUISlider;
class Texture;

class GUIColorPicker :public GUIElement
{
private:
	glm::vec4 primaryColor,secondaryColor;
	glm::vec3 colHSV;
	glm::vec2 cursorPos;
	GUIImage *cursor, *picker;
	GUIPane *bg;
	GUIButton *btnSet,*btnSwitchColor;
	GUIEditBox *ebR, *ebG, *ebB, *ebA;
	GUISlider *sat, *val;
	ImagePtr imgBuf;
	std::shared_ptr<Texture> texBuf;

	void UpdateValues();
	void UpdateColorTextValues();
public:
	GUIColorPicker(GUIEnvironment* env, Rect2D<int> dimensions, bool drawbackground = true);
	virtual ~GUIColorPicker();

	void Render();

	glm::vec4 GetPrimaryColor();
	glm::vec4 GetPrimaryColorGL();

	glm::vec4 GetSecondaryColor();
	glm::vec4 GetSecondaryColorGL();

	void SetPrimaryColorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void SetPrimaryColorRGB(uint8_t r, uint8_t g, uint8_t b);
	void SetSecondaryColorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void SetSecondaryColorRGB(uint8_t r, uint8_t g, uint8_t b);

	//    glm::vec3 GetColorHSV();
	//    void SetColorHSV(uint32_t H, float S, float V);

	void GenerateHSVMap(float s);

	virtual bool OnEvent(const GUIEvent & e);
protected:
};

#endif // GUICOLORPICKER_H
