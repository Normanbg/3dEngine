#ifndef __FONTMANAGER_H__
#define __FONTMANAGER_H__
#include <string>
#include <list>
#include "Globals.h"

#include "MathGeoLib/Math/float2.h"




#define DEFAULT_FONT "arial.ttf"

class Font {
public:
	std::string fontDir = std::string(FONTS_PATH);
	std::string fontSrc = std::string(DEFAULT_FONT);
	std::string exportTexPath = std::string(LIB_FONTS_PATH);
	std::string text = std::string("Insert Text.");
	float scale = 1.0f;
	bool loaded = false;
	void ResetFont();//
	FT_Face textFont;
	void GenerateCharsList();
};

struct Character {

	uint textureID;  // ID handle of the glyph texture
	float2 size;       // Size of glyph
	float2 bearing;    // Offset from baseline to left/top of glyph
	uint advance;    // Offset to advance to next glyph
};

class FontManager
{
public:
	FontManager();
	~FontManager();

	std::list<std::string> loadedFonts = std::list<std::string>();
};


#endif // !__FONTMANAGER_H__
