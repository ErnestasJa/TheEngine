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

enum CustomColorPickerEvents :uint32_t
{
	color_primary = 256,
	color_secondary
};

class GUIColorPicker :public GUIElement
{
private:
	glm::vec4 primaryColor,secondaryColor;
	glm::vec3 colHSV;
	glm::vec2 cursorPos;
	GUIImage *cursor, *picker;
	GUIPane *bg;
	GUIButton *btnSet,*btnSwitchColor;
	GUIEditBox *ebR, *ebG, *ebB;
	GUISlider *sat, *val;
	ImagePtr imgBuf;
	std::shared_ptr<Texture> texBuf;

	void UpdateValues();
public:
	GUIColorPicker(GUIEnvironment* env, Rect2D<int> dimensions, bool drawbackground = true);
	virtual ~GUIColorPicker();

	void Render();

	glm::vec4 GetPrimaryColorRGB();

	glm::vec4 GetPrimaryColorRGBGL();

	glm::vec4 GetSecondaryColorRGB();

	glm::vec4 GetSecondaryColorRGBGL();

	void SetPrimaryColorRGB(uint8_t r, uint8_t g, uint8_t b);
	void SetSecondaryColorRGB(uint8_t r, uint8_t g, uint8_t b);

	//    glm::vec3 GetColorHSV();
	//    void SetColorHSV(uint32_t H, float S, float V);

	void GenerateHSVMap(float s);

	virtual bool OnEvent(const GUIEvent & e);
protected:
};

#endif // GUICOLORPICKER_H
