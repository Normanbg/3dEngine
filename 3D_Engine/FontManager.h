#ifndef __FONTMANAGER_H__
#define __FONTMANAGER_H__
#include <string>
#include <list>
#include <map>
#include "Globals.h"

#include "MathGeoLib/Math/float2.h"
#include "Glew/include/glew.h"

#include <ft2build.h>                                              
#include FT_FREETYPE_H

#pragma comment(lib, "FreeType/win32/freetype.lib") 


#define DEFAULT_FONT "Crimson-Roman.ttf"
#define DEFAULT_SCALE 30

struct Character {

	uint textureID;  // ID handle of the glyph texture
	float2 size;       // Size of glyph
	float2 bearing;    // Offset from baseline to left/top of glyph
	uint advance;    // Offset to advance to next glyph

};
class Font {
public:
	std::string fontDir = std::string(FONTS_PATH);
	std::string fontSrc = std::string(DEFAULT_FONT);
	

	Font(const char* name );
	void GenerateCharsList();
	uint GetCharacterTexture(GLchar character);
	Character * GetCharacter(GLchar character);
	
	void CleanUp();
	FT_Face face;
	float scale = 1.0f;
	std::map<GLchar, Character*> charsList;
};


class FontManager
{
public:
	FontManager();
	~FontManager();

	
	void CleanUp();
	

	void LoadAllFolderFonts();

	Font * LoadFont(const char * name, uint size);
	Font* GetFont(const char * name);
	void RemoveFont(const char * name);
	FT_Library library;
	std::vector<Font*> loadedFonts = std::vector<Font*>();
};


#endif // !__FONTMANAGER_H__
