#include "FontManager.h"
#include "Application.h"

#include "MathGeoLib/Math/float3.h"
#include "ModuleFileSystem.h"

#include "mmgr/mmgr.h"



FontManager::FontManager()
{
	library = FT_Library();
	FT_Init_FreeType(&library);
	
}


FontManager::~FontManager()
{
}

void FontManager::CleanUp()
{
	for (std::vector<Font*>::reverse_iterator it = loadedFonts.rbegin(); it != loadedFonts.rend(); it++)
	{		
		(*it)->CleanUp();		
		RELEASE((*it));
	}
	loadedFonts.clear();
}



Font::Font(const char * name)
{
	if(name)
		fontSrc = name;
}

void Font::GenerateCharsList()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

	float3 cursor = { 0,0,0 };

	for (GLubyte c = 32; c < 128; c++)
	{
		Character* nwChar = new Character();
		
		
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			OWN_LOG("ERROR::FREETYTPE: Failed to load Glyph");
			continue;
		}
				
		glGenTextures(1, &nwChar->textureID);		
		glBindTexture(GL_TEXTURE_2D, nwChar->textureID);
		glTexImage2D(GL_TEXTURE_2D,0, GL_ALPHA,face->glyph->bitmap.width,face->glyph->bitmap.rows,0, GL_ALPHA,GL_UNSIGNED_BYTE,face->glyph->bitmap.buffer);
		
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			OWN_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		
		nwChar->bearing = { (float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top };
		nwChar->size = { (float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows };
		nwChar->advance = face->glyph->advance.x / 64.0f;

		charsList.insert(std::pair<char, Character*>(c, nwChar));		
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

void Font::CleanUp()
{
	for (std::map<GLchar, Character*>::reverse_iterator it = charsList.rbegin(); it != charsList.rend(); it++)
	{		
		glDeleteBuffers(1, &(*it).second->textureID);
		RELEASE((*it).second);
	}
	charsList.clear();
	FT_Done_Face(face);
}

void FontManager::LoadAllFolderFonts()
{
	std::vector<std::string> fonts;
	App->fileSys->GetFilesFromDir(FONTS_PATH, fonts, fonts); // load fonts
	for (int i = 0; i < fonts.size(); i++) {
		std::string name = std::string();
		App->fileSys->GetNameFromPath(fonts[i].c_str(), nullptr, nullptr, &name, nullptr);
 		LoadFont(name.c_str(), DEFAULT_SCALE);
	}
}

Font* FontManager::LoadFont(const char* name, uint scale)
{
	Font* font = new Font(name);

	std::string path = font->fontDir + font->fontSrc;
	FT_Error error = FT_New_Face(library, path.c_str(), 0, &font->face);

	if (!error){
		
		FT_Set_Pixel_Sizes(font->face, 0, scale);
		font->GenerateCharsList();
		font->scale = scale;
		loadedFonts.push_back(font);	
		bool add = true;
		for (int i = 0; i < singleFonts.size(); i++) {
			if (font->fontSrc == singleFonts[i]) {
				add = false;
				break;
			}
		}
		if (add) { singleFonts.push_back(font->fontSrc.c_str()); }
	}
	else {		
		font->CleanUp();		
		RELEASE(font);
		FT_Done_FreeType(library);
	}
	
	return font;
}

Font* FontManager::GetFont(const char* _name)
{
	for (std::vector<Font*>::iterator it = loadedFonts.begin(); it != loadedFonts.end(); it++)
	{
		if (strcmp((*it)->fontSrc.c_str(), _name)==0)
		{
			return (*it);
		}
	}

	return nullptr;
}

void FontManager::RemoveFont(const char* name)
{
	for (std::vector<Font*>::iterator it = loadedFonts.begin(); it != loadedFonts.end(); it++)
	{
		if (strcmp((*it)->fontSrc.c_str(), name)==0)
		{
			(*it)->CleanUp();
			loadedFonts.erase(it);
			return;
		}
	}
}
