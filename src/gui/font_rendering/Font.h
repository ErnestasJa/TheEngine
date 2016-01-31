#pragma once

#include "ForwardDecl.h"

#define MAX_FONT_ATLAS_WIDTH 1024
#define MAX_FONT_ATLAS_HEIGHT 1024
#define GLYPHS_PER_ATLAS 1024

struct FontFamily;

struct Font {
    FontFamily *myFamily;

    TexturePtr atlas;

    std::string name;

    int w;  // width of Texture in pixels
    int h;  // height of Texture in pixels
    float avgheight;
    int realHeight;

    struct SGlyphInfo {
        ~SGlyphInfo()
        {
            delete bitmap;
        }

        float ax;  // advance.x
        float ay;  // advance.y

        float bw;  // bitmap.width;
        float bh;  // bitmap.height;

        float bl;  // bitmap_left;
        float bt;  // bitmap_top;

        float tx;  // x offset of glyph in Texture coordinates
        float ty;  // y offset of glyph in Texture coordinates

        uint8_t *bitmap;
    } c[GLYPHS_PER_ATLAS];  // character information

    Font(FT_Face face, int height, std::string name);

    ~Font();
};
