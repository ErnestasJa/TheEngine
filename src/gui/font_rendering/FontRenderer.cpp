#include "Precomp.h"
#include "utility/Helpers.h"
#include "application/AppContext.h"
#include "utility/Logger.h"
#include "opengl/material/Shader.h"
#include "resources/ResourceManager.h"
#include "opengl/OpenGLExtensionLoader.h"
#include "gui/GUIEnvironment.h"
#include "FontRenderer.h"
#include "opengl/Texture.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include "resources/Image.h"

FontRenderer::FontRenderer()
{
    this->_guiEnvironment = GetContext().GetGUIEnvironment();
    this->_logger = GetContext().GetLogger();

    FT_Error err = FT_Init_FreeType(&_ftLib);

    if (err) {
        _logger->log(LOG_ERROR, "Error on init:%u\n", err);
        exit(-1);
    }

    _defaultFamily = CreateFontFamily("default", 12, "res/gui/fonts/OpenSans-Regular.ttf", "res/gui/fonts/OpenSans-Bold.ttf", "res/gui/fonts/OpenSans-Italic.ttf", "res/gui/fonts/OpenSans-BoldItalic.ttf");
    if (_defaultFamily == nullptr) exit(-1);
    UseFontFamily(_defaultFamily->familyName);
    UseFont(FFT_REGULAR);
    if (_currentFamily != _defaultFamily) exit(-1);

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    _fontShader = GetContext().GetResourceManager()->LoadShader(Path("res/engine/shaders/font"));
    printf("Font shader id:%d\n", _fontShader->GetProgramId());
}

FontRenderer::~FontRenderer()
{
    //    for(fvi i=fonts.begin(); i!=fonts.end(); i++)
    //    {
    //        delete (*i);
    //    }
    while (_fontFamilies.size() > 0) {
        auto family = _fontFamilies.begin();
        delete family->second;
        _fontFamilies.erase(family);
    }
    _fontFamilies.clear();
    FT_Done_FreeType(_ftLib);
    _fontShader = nullptr;
}

Font* FontRenderer::_CreateFont(const std::string& name, const std::string& filename, const int32_t& size)
{
    char* buf = NULL;

    uint32_t fl = helpers::read(filename, buf);

    _logger->log(LOG_DEBUG, "BUF: %u\n", fl);

    if (fl <= 0) {
        _logger->log(LOG_ERROR, "Font file appears to be empty or corrupt.\n");
        exit(-1);
    }

    FT_Error err = FT_New_Memory_Face(_ftLib, reinterpret_cast<FT_Byte*>(buf), fl, 0, &_ftFace);
    if (err) {
        _logger->log(LOG_ERROR, "Error on creating a font: %d\n", err);
        exit(-1);
    }

    Font* temp = new Font(_ftFace, size, name);

    delete buf;

    FT_Done_Face(_ftFace);

    return temp;
}

FontFamily* FontRenderer::CreateFontFamily(const std::string& name, uint32_t size, std::string regularFileName, std::string boldFileName, std::string italicFileName, std::string boldItalicFileName)
{
    FontFamily* check = GetFontFamily(name);
    if (check != nullptr) return check;

    FontFamily* ret = new FontFamily(name);

    if (regularFileName.length() > 0) ret->AddFont(FFT_REGULAR, _CreateFont(name + "-Regular", regularFileName, size));
    if (boldFileName.length() > 0) ret->AddFont(FFT_BOLD, _CreateFont(name + "-Bold", boldFileName, size));
    if (italicFileName.length() > 0) ret->AddFont(FFT_ITALIC, _CreateFont(name + "-Italic", italicFileName, size));
    if (boldItalicFileName.length() > 0) ret->AddFont(FFT_BOLD_ITALIC, _CreateFont(name + "-BoldItalic", boldItalicFileName, size));

    ret->currentType = FFT_REGULAR;

    _fontFamilies[name] = ret;

    return ret;
}

void FontRenderer::UseFontFamily(const std::string& familyName)
{
    if (familyName.length() > 0) {
        FontFamily* candidate = GetFontFamily(familyName);
        if (candidate != nullptr) {
            _currentFamily = candidate;
            UseFont(FFT_REGULAR);
            return;
        }
    }

    _currentFamily = _defaultFamily;
    UseFont(FFT_REGULAR);
}

void FontRenderer::UseFont(FONT_FAMILY_TYPE f)
{
    if (_currentFamily->Has(f)) {
        _currentFont = _currentFamily->GetFamilyFont(f);
        ;
        _currentFamily->currentType = f;
        return;
    }
}

FontFamily* FontRenderer::GetFontFamily(const std::string& name)
{
    if (_fontFamilies.count(name) > 0) return _fontFamilies[name];
    return nullptr;
}

FontFamily* FontRenderer::GetDefaultFontFamily()
{
    return _defaultFamily;
}

FontFamily* FontRenderer::GetCurrentFontFamily()
{
    return _currentFamily;
}

Font* FontRenderer::GetCurrentFont()
{
    return _currentFont;
}

void FontRenderer::SetDefaultFontFamily(const std::string& familyName)
{
    FontFamily* candidate = GetFontFamily(familyName);
    if (candidate != nullptr) _defaultFamily = candidate;
}

int32_t FontRenderer::_FindTagEnd(std::wstring str, const wchar_t tag)
{
    wchar_t buf[32];

    swprintf(buf, 32, L"['%c", tag);
    std::wstring thistagopen = buf;

    swprintf(buf, 32, L"[%c']", tag);
    std::wstring thistagclose = buf;

    int32_t tagstart = str.find(thistagopen);

    size_t naiveend = str.find(thistagclose, tagstart);

    int32_t track = tagstart;
    int32_t realend = naiveend;
    while (1) {
        size_t nestedcheck = str.find(thistagopen, track + 3);
        bool nested = nestedcheck != std::string::npos;

        if (nested && nestedcheck < naiveend) {
            track = str.find(thistagopen, track + 3);
            naiveend = str.find(thistagclose, naiveend + 4);
        } else
            break;
    }
    realend = naiveend;
    return realend;
}

void FontRenderer::_FormatTags(TextLine& tl, std::wstring in, SubLineInfo inf)
{
    if (in.length() == 0) return;
    size_t firsttag = 0, tagclose = 0, tagend = 0, taglength = 0;
    wchar_t tag = L' ';

    firsttag = in.find(L"['", 0);

    if (firsttag != std::string::npos) {
        /// printf("tag found\n");
        tag = in.substr(firsttag + 2, 1)[0];
        tagclose = in.find(L"]", firsttag);
        tagend = _FindTagEnd(in, tag);
        taglength = tagend - (tagclose + 1);
    } else  /// apparently, no tags
    {
        /// printf("no tag found\n");
        if (in.length() > 0) {
            inf.text = in;
            tl.content.push_back(inf);
        }
        return;
    }
    /// do we have pre-text? if yes, cut it and try again. keep formatting data.
    if (firsttag != 0) {
        inf.text = in.substr(0, firsttag);
        tl.content.push_back(inf);
        in = in.substr(firsttag);
        /// wprintf(L"pretag found applying formatting if any\nsubstr:%ls\n",in.c_str());
        _FormatTags(tl, in, inf);
        return;
    }

    SubLineInfo oldinf = inf;
    switch (tag) {
        case L'c': {
            std::vector<std::wstring> tagvals;
            std::wstring tagvalsubstr = in.substr(firsttag + 4, tagclose - 4);
            boost::split(tagvals, tagvalsubstr, boost::is_any_of(L", "));

            float r, g, b, a;

            r = 1.f / 255.f * helpers::wtoi(tagvals[0].c_str());
            g = 1.f / 255.f * helpers::wtoi(tagvals[1].c_str());
            b = 1.f / 255.f * helpers::wtoi(tagvals[2].c_str());

            if (tagvals.size() == 4) {
                a = 1.f / 255.f * helpers::wtoi(tagvals[3].c_str());
            } else {
                a = 1.f;
            }

            inf.color = glm::vec4(r, g, b, a);
        } break;
        case L's': {
            inf.shadow = true;
        } break;
        case L'b': {
            inf.bold = true;
        } break;
        case L'i': {
            inf.italic = true;
        } break;
    }

    std::wstring before = in.substr(tagclose + 1, taglength);
    std::wstring after = in.substr(tagend + 4);
    in = before + after;
    _FormatTags(tl, before, inf);
    _FormatTags(tl, after, oldinf);
    return;
}

Image* FontRenderer::RenderStringToImage(const std::wstring& text, glm::detail::tvec3<uint8_t> color, std::string fontFamilyName)
{
    this->UseFontFamily(fontFamilyName);
    Font* a = _currentFont;

    glm::vec2 textDimensions = this->GetTextDimensions(text);

    Image* img = new Image();

    int imgW = (int)glm::ceil(textDimensions.x);
    int imgH = (int)glm::ceil(textDimensions.y);

    img->Init(imgW, imgH, 4);

    glm::vec2 pos(0);

    /* Loop through all characters */
    for (size_t i = 0; i < text.length(); i++) {
        const uint16_t p = (const uint16_t)text.c_str()[i];

        /* Calculate the vertex and Texture coordinates */
        float x2 = pos.x;
        float y2 = pos.y;
        float w = a->c[p].bw;
        float h = a->c[p].bh;
        uint8_t* bmp = a->c[p].bitmap;

        /* Advance the cursor to the start of the next character */
        pos.x += a->c[p].ax;
        pos.y += a->c[p].ay;

        /* Skip glyphs that have no pixels */
        if (!w || !h) continue;

        loop(y, h)
        {
            loop(x, w)
            {
                uint8_t data = bmp[y * (int)w + x];

                int xw = x2 + x;
                int yw = h - y2 - y;

                if (x2 + x > imgW) printf("out of width bounds\n");

                if (pos.y + h > imgH) printf("out of height bounds\n");

                img->SetPixel(xw, yw, color.r, color.g, color.b, data);
            }
        }
    }

    return img;
}

void FontRenderer::_RenderString(const std::wstring& text, glm::ivec2 pos, const glm::vec4& color)
{
    glm::vec2 gs = _guiEnvironment->GetGUIScaling();
    float sx, sy;
    sx = gs.x;
    sy = gs.y;

    glm::vec2 _pos = (glm::vec2)pos;

    _pos.x = -1 + _pos.x * sx;
    _pos.y = 1 - _pos.y * sy - _currentFont->avgheight * sy;

    /* Use the Texture containing the atlas */
    _currentFont->atlas->Set(0);
    _SetFontColor(color);
    SetBindingSafe(_fontShader, "tex", 0);
    _fontShader->Set();

    glBindVertexArray(_VAO);

    /* Set up the VBO for our vertex data */
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    vector<glm::vec4> coords;
    coords.resize(text.length() * 6);

    int c = 0;

    Font* a = _currentFont;

    /* Loop through all characters */
    for (int i = 0; i < text.length(); i++) {
        const uint16_t p = (const uint16_t)text.c_str()[i];

        /* Calculate the vertex and Texture coordinates */
        float x2 = _pos.x + a->c[p].bl * sx;
        float y2 = -_pos.y - a->c[p].bt * sy;
        float w = a->c[p].bw * sx;
        float h = a->c[p].bh * sy;

        /* Advance the cursor to the start of the next character */
        _pos.x += a->c[p].ax * sx;
        _pos.y += a->c[p].ay * sy;

        /* Skip glyphs that have no pixels */
        if (!w || !h) continue;

        coords[c++] = glm::vec4(x2, -y2, a->c[p].tx, a->c[p].ty);
        coords[c++] = glm::vec4(x2, -y2 - h, a->c[p].tx, a->c[p].ty + a->c[p].bh / a->h);
        coords[c++] = glm::vec4(x2 + w, -y2, a->c[p].tx + a->c[p].bw / a->w, a->c[p].ty);

        coords[c++] = glm::vec4(x2, -y2 - h, a->c[p].tx, a->c[p].ty + a->c[p].bh / a->h);
        coords[c++] = glm::vec4(x2 + w, -y2 - h, a->c[p].tx + a->c[p].bw / a->w, a->c[p].ty + a->c[p].bh / a->h);
        coords[c++] = glm::vec4(x2 + w, -y2, a->c[p].tx + a->c[p].bw / a->w, a->c[p].ty);
    }

    /* Draw all the character on the screen in one go */
    glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(glm::vec4), glm::value_ptr(coords[0]), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, c);

    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

void FontRenderer::_RenderString(const std::wstring& text, glm::ivec2 pos, const glm::vec4& color, bool drawShadow)
{
    if (drawShadow) _RenderString(text, pos + glm::ivec2(1), glm::vec4(0, 0, 0, color.w));
    _RenderString(text, pos, color);
}

/// Planned flow:
/// Split by newlines
/// Strip pre-tag text, strip after-tag text, strip and set tag info
/// Check for length wrapping failures, newline accordingly with the same data used for the formatted lines
/// Regex case: <([a-zA-Z][A-Z0-9]*)\b[^>]*>(.*?)</\1>
void FontRenderer::RenderString(const std::wstring& text, const glm::ivec2& pos, std::string fontFamilyName)
{
    FontFamily* old = _currentFamily;

    if (GetFontFamily(fontFamilyName) != nullptr) UseFontFamily(fontFamilyName);

    FontFamily* current = _currentFamily;
    FONT_FAMILY_TYPE currentStyle = current->currentType;
    bool canbold = current->Has(FFT_BOLD);
    bool canitalic = current->Has(FFT_ITALIC);
    bool canbolditalic = current->Has(FFT_BOLD_ITALIC);

    vector<std::wstring> strs;
    boost::split(strs, text, boost::is_any_of(L"\n"));
    if (strs.size() == 0) strs.push_back(text);

    vector<TextLine> linesToDraw;
    linesToDraw.resize(strs.size());

    SubLineInfo inf;

    loop(i, strs.size())
    {
        _FormatTags(linesToDraw[i], strs[i], inf);
    }

    // ReadAndStripTags(strs,linesToDraw);
    // printf("LinesToDraw: %d\n",linesToDraw.size());
    loop(i, linesToDraw.size())
    {
        glm::vec2 dims = glm::vec2(0, _currentFont->avgheight);
        TextLine _current = linesToDraw[i];
        loop(j, _current.content.size())
        {
            SubLineInfo _celem = _current.content[j];
            if (_celem.bold && canbold && !_celem.italic) {
                currentStyle = current->currentType;
                UseFont(FFT_BOLD);
            } else if (_celem.italic && canitalic && !_celem.bold) {
                currentStyle = current->currentType;
                UseFont(FFT_ITALIC);
            } else if (_celem.bold && _celem.italic && canbolditalic) {
                currentStyle = current->currentType;
                UseFont(FFT_BOLD_ITALIC);
            }
            if (j != 0) {
                _RenderString(_celem.text, pos + glm::ivec2(dims.x, i * (dims.y + dims.y / 2.f)), _celem.color, _celem.shadow);
            } else {
                _RenderString(_celem.text, pos + glm::ivec2(0, i * (dims.y + dims.y / 2.f)), _celem.color, _celem.shadow);
            }
            dims = glm::vec2(dims.x + GetTextDimensions(_celem.text).x, dims.y);
            if (_celem.bold || _celem.italic) {
                UseFont(currentStyle);
            }
        }
    }
    if (GetFontFamily(fontFamilyName) != nullptr) {
        UseFontFamily(old->familyName);
        UseFont(currentStyle);
    }
}

void FontRenderer::_SetFontColor(const glm::vec4& color)
{
    SetBindingSafe(_fontShader, "color", color);
}

glm::vec2 FontRenderer::GetTextDimensions(const std::wstring& text)
{
    TextLine lineToDraw;

    SubLineInfo inf;

    _FormatTags(lineToDraw, text, inf);

    float len = 0;
    float height = 0;

    FONT_FAMILY_TYPE currentStyle = _currentFamily->currentType;
    bool canbold = _currentFamily->Has(FFT_BOLD);
    bool canitalic = _currentFamily->Has(FFT_ITALIC);
    bool canbolditalic = _currentFamily->Has(FFT_BOLD_ITALIC);

    for (auto& content : lineToDraw.content) {
        if (content.bold && canbold && !content.italic) {
            currentStyle = _currentFamily->currentType;
            UseFont(FFT_BOLD);
        } else if (content.italic && canitalic && !content.bold) {
            currentStyle = _currentFamily->currentType;
            UseFont(FFT_ITALIC);
        } else if (content.bold && content.italic && canbolditalic) {
            currentStyle = _currentFamily->currentType;
            UseFont(FFT_BOLD_ITALIC);
        }
        for (wchar_t& glyph : content.text) {
            auto& glyphData = _currentFont->c[(int)glyph];
            len += glyphData.ax;
        }

        height = _currentFont->avgheight;

        if (height < _currentFont->realHeight) {
            height = _currentFont->realHeight;
        }

        if (content.bold || content.italic) {
            UseFont(currentStyle);
        }
    }

    return glm::vec2(len, height);
}