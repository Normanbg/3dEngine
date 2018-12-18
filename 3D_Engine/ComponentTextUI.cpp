#include "ComponentTextUI.h"
#include "ModuleFileSystem.h"


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "TrueType/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION 
#include "TrueType/stb_truetype.h"

#include "mmgr/mmgr.h"




ComponentTextUI::ComponentTextUI()
{
	typeUI = UI_TEXT;
	type = UI;
}


ComponentTextUI::~ComponentTextUI()
{
}

bool ComponentTextUI::Start()
{
	char* buffer = nullptr;
	fontPath += "arial.ttf";
	int size = App->fileSys->readFile(fontPath.c_str(), &buffer);

	stbtt_fontinfo info;
	if (!stbtt_InitFont(&info, (unsigned char*) buffer, 0))
	{
		OWN_LOG("failed initialising font");
	}

	int b_w = 512; /* bitmap width */
	int b_h = 128; /* bitmap height */
	int l_h = 64; /* line height */

	unsigned char* bitmap = new unsigned char[b_w * b_h]; // creates bitmap for the phrase
	
	scale = stbtt_ScaleForPixelHeight(&info, l_h);// calculate font scaling

	text = "I'm working correctly!";
	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

	ascent *= scale;
	descent *= scale;

	int x = 0;
	int i;
	for (i = 0; i < strlen(text); ++i)
	{
		/* get bounding box for character (may be offset to account for chars that dip above or below the line */
		int c_x1, c_y1, c_x2, c_y2;
		stbtt_GetCodepointBitmapBox(&info, text[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

		/* compute y (different characters have different heights */
		int y = ascent + c_y1;

		/* render character (stride and offset is important here) */
		int byteOffset = x + (y  * b_w);
		stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, text[i]);

		/* how wide is this character */
		int ax;
		stbtt_GetCodepointHMetrics(&info, text[i], &ax, 0);
		x += ax * scale;

		/* add kerning */
		int kern;
		kern = stbtt_GetCodepointKernAdvance(&info, text[i], text[i + 1]);
		x += kern * scale;
	}
	stbi_write_png("plsWork2.png", b_w, b_h, 1, bitmap, b_w);
	RELEASE_ARRAY(buffer);
	RELEASE_ARRAY(bitmap);
	return true;
}

bool ComponentTextUI::Update()
{
	return true;
}

void ComponentTextUI::CleanUp()
{
}

void ComponentTextUI::Load(Config * data)
{
	UUID = data->GetUInt("UUID");
}

void ComponentTextUI::Save(Config & data) const
{
	data.AddUInt("UUID", UUID);
}

void ComponentTextUI::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
}
