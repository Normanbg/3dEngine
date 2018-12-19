#include "ComponentTextUI.h"
#include "ModuleFileSystem.h"
#include "TextureImporter.h"
#include "GameObject.h"
#include "ComponentRectTransform.h"

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

	static const float uvs[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};

	texCoords = new float2[4];
	memcpy(texCoords, uvs, sizeof(float2) * 4);

	rectTransform = myGO->GetComponentRectTransform();
	
	LoadLabel();
	return true;
}

bool ComponentTextUI::Update()
{
	return true;
}

void ComponentTextUI::CleanUp()
{
	//LEFT clean buffer
	rectTransform = nullptr;
	
	myGO = nullptr;
	RELEASE_ARRAY(texCoords);
}

void ComponentTextUI::Load(Config * data)
{
	UUID = data->GetUInt("UUID");
}

void ComponentTextUI::Save(Config & data) const
{
	data.AddUInt("UUID", UUID);
}

void ComponentTextUI::LoadLabel(const char * _label, uint _scale, const char * _font)
{
	//-------- Loading Font arial -->
	font.ResetFont();
	char* buffer = nullptr;
	font.fontPath += _font;
	int size = App->fileSys->readFile(font.fontPath.c_str(), &buffer);

	stbtt_fontinfo info;
	if (!stbtt_InitFont(&info, (unsigned char*)buffer, 0))
	{
		OWN_LOG("failed initialising font");
		return;
	}

	int b_w = 512; // bitmap width 
	int b_h = 128; // bitmap height
	int l_h = 64; // line height 

	unsigned char* bitmap = new unsigned char[b_w * b_h]; // creates bitmap for the phrase

	font.scale = stbtt_ScaleForPixelHeight(&info, l_h)*_scale;// calculate font scaling

	font.text = (char*)_label;
	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

	ascent *= font.scale;
	descent *= font.scale;

	int x = 0;
	int i;
	for (i = 0; i < strlen(font.text); ++i)
	{
		/* get bounding box for character (may be offset to account for chars that dip above or below the line */
		int c_x1, c_y1, c_x2, c_y2;
		stbtt_GetCodepointBitmapBox(&info, font.text[i], font.scale, font.scale, &c_x1, &c_y1, &c_x2, &c_y2);

		/* compute y (different characters have different heights */
		int y = ascent + c_y1;

		/* render character (stride and offset is important here) */
		int byteOffset = x + (y  * b_w);
		stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, font.scale, font.scale, font.text[i]);

		/* how wide is this character */
		int ax;
		stbtt_GetCodepointHMetrics(&info, font.text[i], &ax, 0);
		x += ax * font.scale;

		/* add kerning */
		int kern;
		kern = stbtt_GetCodepointKernAdvance(&info, font.text[i], font.text[i + 1]);
		x += kern * font.scale;
	}

	font.exportTexPath += (std::to_string(UUID) + PNG_FORMAT);
	stbi_write_png(font.exportTexPath.c_str(), b_w, b_h, 1, bitmap, b_w);
	RELEASE_ARRAY(buffer);
	RELEASE_ARRAY(bitmap);
	uint texW, texH;
	texGPUIndex = App->texImporter->LoadTexture(font.exportTexPath.c_str(), texW, texH);
}


void ComponentTextUI::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
	static const int maxSize = 16;
	std::string name = std::string(font.text);
	if (ImGui::InputText("Label Text",(char*) name.c_str(), maxSize)) {
		LoadLabel(name.c_str(),2);
	}
}

void ComponentTextUI::DrawUI()
{
	glPushMatrix();
	glMultMatrixf(rectTransform->GetGlobalMatrix().Transposed().ptr());

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer
	glBindTexture(GL_TEXTURE_2D, 0);

	glLineWidth(4.0f);

	if (texGPUIndex !=-1) {

		glBindTexture(GL_TEXTURE_2D, texGPUIndex);
		glTexCoordPointer(2, GL_FLOAT, 0, &(texCoords[0]));

	}
	glBindBuffer(GL_ARRAY_BUFFER, rectTransform->GetVertexID());
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_QUADS, 0, 4);

	glLineWidth(1.0f);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void ComponentTextUI::Font::ResetFont()
{
	fontPath = std::string(FONTS_PATH);
	exportTexPath = std::string(LIB_FONTS_PATH);
	scale = 1.0f;
	text = "Insert Text";
}
