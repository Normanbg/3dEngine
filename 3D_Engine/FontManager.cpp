#include "FontManager.h"
#include "Glew/include/glew.h"
#include "MathGeoLib/Math/float3.h"

#include "FreeType/include/ft2build.h"
#include "FreeType/include/freetype/freetype.h"

#pragma comment(lib, "FreeType/win32/freetype.lib") 

FontManager::FontManager()
{
}


FontManager::~FontManager()
{
}

void Font::GenerateCharsList()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	float3 cursor = { 0,0,0 };

	for (GLubyte c = 0; c < 128; c++)
	{
		Character* new_character = new Character();

		// Load character glyph 
		if (FT_Load_Char(text_font, c, FT_LOAD_RENDER))
		{
			OWN_LOG("ERROR::FREETYTPE: Failed to load Glyph");
			continue;
		}

		// Generate texture
		glGenTextures(1, &new_character->TextureID);
		glBindTexture(GL_TEXTURE_2D, new_character->TextureID);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			text_font->glyph->bitmap.width,
			text_font->glyph->bitmap.rows,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			text_font->glyph->bitmap.buffer
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		new_character->Bearing = { (float)text_font->glyph->bitmap_left, (float)text_font->glyph->bitmap_top };
		new_character->Size = { (float)text_font->glyph->bitmap.width, (float)text_font->glyph->bitmap.rows };
		new_character->Advance = text_font->glyph->advance.x / 64.0f;

		chars_list.insert(std::pair<GLchar, Character*>(c, new_character));
		loaded = true;
	}
}