#include "FontManager.h"
#include "MathGeoLib/Math/float3.h"





FontManager::FontManager()
{
	library = FT_Library();
	FT_Init_FreeType(&library);
	//LoadFont(DEFAULT_FONT,20);
	
}


FontManager::~FontManager()
{
}

Font::Font(const char * name)
{
	if(name)
		fontSrc = name;
}

void Font::GenerateCharsList()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	float3 cursor = { 0,0,0 };

	for (GLubyte c = 32; c < 128; c++)
	{
		Character* new_character = new Character();

		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			OWN_LOG("ERROR::FREETYTPE: Failed to load Glyph");
			continue;
		}

		// Generate texture
		
		glGenTextures(1, &new_character->textureID);
		glBindTexture(GL_TEXTURE_2D, new_character->textureID);

		glTexImage2D(GL_TEXTURE_2D,0, GL_ALPHA,face->glyph->bitmap.width,face->glyph->bitmap.rows,0, GL_ALPHA,GL_UNSIGNED_BYTE,face->glyph->bitmap.buffer);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		new_character->bearing = { (float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top };
		new_character->size = { (float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows };
		new_character->advance = face->glyph->advance.x / 64.0f;

		charsList.insert(std::pair<char, Character*>(c, new_character));
		loaded = true;
	}
}
uint Font::GetCharacterTexture(GLchar character)
{
	for(std::map<GLchar, Character*>::iterator it = charsList.begin(); it!=charsList.end(); it++)
	{
		if ((*it).first ==(GLchar)character)
		{
			uint id = (*it).second->textureID;
			return id;
		}
	}
}

Character* Font::GetCharacter(GLchar character) 
{
	for (std::map<GLchar, Character*>::iterator it = charsList.begin(); it != charsList.end(); it++)
	{
		if ((*it).first == (GLchar)character)
		{
			return  (*it).second;
		}
	}
	return nullptr;
}

Font* FontManager::LoadFont(const char* name, uint scale)
{
	Font* font = new Font(name);

	std::string path = font->fontDir + font->fontSrc;
	FT_Error error = FT_New_Face(library, path.c_str(), 0, &font->face);

	if (!error){
		/*if (FT_HAS_VERTICAL(font->face))
		{
			OWN_LOG("Vertical fonts not supported");
			font.cleanup
			FT_Done_FreeType(library);
			return nullptr;
		}*/

		FT_Set_Pixel_Sizes(font->face, 0, scale);
		font->GenerateCharsList();
		font->scale = scale;
		loadedFonts.push_back(font);		
	}
	else {		
		font->CleanUp();		
		RELEASE(font);
		FT_Done_FreeType(library);
	}
	
	return font;
}

Font FontManager::GetFont(const char* _name)
{
	for (std::vector<Font*>::iterator it = loadedFonts.begin(); it != loadedFonts.end(); it++)
	{
		if (strcmp((*it)->fontSrc.c_str(), _name)==0)
		{
			return *(*it);
		}
	}

	return nullptr;
}