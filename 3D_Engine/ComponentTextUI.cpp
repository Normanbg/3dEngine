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
	App->fileSys->GetFilesFromDir(FONTS_PATH, loadedFonts, loadedFonts); // load fonts

	static const float uvs[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};

	texCoords = new float2[4];
	memcpy(texCoords, uvs, sizeof(float2) * 4);

	rectTransform = myGO->GetComponentRectTransform();
	rectTransform->SetWidth(3.0f);
	font.exportTexPath += (std::to_string(UUID) + PNG_FORMAT);
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

void ComponentTextUI::LoadLabel(const char * _label, float _scale, const char * _font)
{
	//-------- Loading Font arial -->
	//font.ResetFont();	

	font.fontSrc = _font;
	font.scale = _scale;
	font.text =_label;

	std::string fullFontPath = font.fontDir + font.fontSrc;

	char* buffer = nullptr;
	int size = App->fileSys->readFile(fullFontPath.c_str(), &buffer);

	stbtt_fontinfo info;
	if (!stbtt_InitFont(&info, (unsigned char*)buffer, 0))
	{
		OWN_LOG("failed initialising font");
		return;
	}

	int bm_w = 100*rectTransform->GetWidth(); // bitmap width 
	int bm_h = 100*rectTransform->GetHeight(); // bitmap height
	int line_h = 64; // line height 
	if (bm_w <= 0 || bm_h <= 0) { return; }
	unsigned char* bitmap = new unsigned char[bm_w * bm_h]; // creates bitmap for the phrase
	float sc = stbtt_ScaleForPixelHeight(&info, line_h)*font.scale;// calculate font scaling

	
	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap); // get lowest and upper height

	ascent *= sc;
	descent *=sc;

	int x = 0;	
	for (int i = 0; i < font.text.size(); ++i)
	{
		
		int c_x1, c_y1, c_x2, c_y2;
		stbtt_GetCodepointBitmapBox(&info, font.text[i], sc, sc, &c_x1, &c_y1, &c_x2, &c_y2);// get bounding box for character (may be offset to account for chars that dip above or below the line)

		
		int y = ascent + c_y1; //calculates y bc every character has its own height

		
		int byteOffset = x + (y  * bm_w);
		stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, bm_w, sc, sc, font.text[i]);// render character (stride and offset is important here) 

		
		int ax;
		stbtt_GetCodepointHMetrics(&info, font.text[i], &ax, 0); //  character's wide
		x += ax * sc;

		
		int kern;
		kern = stbtt_GetCodepointKernAdvance(&info, font.text[i], font.text[i + 1]);// add kerning 
		x += kern * sc;
	}

	
	stbi_write_png(font.exportTexPath.c_str(), bm_w, bm_h, 1, bitmap, bm_w); // export to PNG
	RELEASE_ARRAY(buffer);
	RELEASE_ARRAY(bitmap);
	uint texW, texH;
	texGPUIndex = App->texImporter->LoadTexture(font.exportTexPath.c_str(), texW, texH); // load the texture
}



void ComponentTextUI::UpdateRectTransform()
{
	LoadLabel(font.text.c_str(), font.scale, font.fontSrc.c_str());
}

void ComponentTextUI::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
	static const int maxSize = 16;
	if (ImGui::InputText("Label Text",(char*) font.text.c_str(), maxSize)|| ImGui::SliderFloat("Scale", &font.scale, 0.1f, 1.5f)) {
		LoadLabel(font.text.c_str(),font.scale, font.fontSrc.c_str());
	}
	if (ImGui::BeginCombo("Fonts", font.fontSrc.c_str()))
	{
		for (int i = 0; i < loadedFonts.size(); i++)
		{
			bool isSelected = false;
			if (strcmp(font.fontSrc.c_str(), loadedFonts[i].c_str()) == 0) {
				isSelected = true;
			}
			
			if (ImGui::Selectable(loadedFonts[i].c_str(), isSelected)) {
				std::string chosenFont;
				App->fileSys->GetNameFromPath(loadedFonts[i].c_str(), nullptr, nullptr, &chosenFont, nullptr);
				font.fontSrc = chosenFont;
				LoadLabel(font.text.c_str(), font.scale, font.fontSrc.c_str());

				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}

			}

		}
		ImGui::EndCombo();
	}
}

void ComponentTextUI::DrawUI()
{
	glPushMatrix();
	float4x4 globalMat;
	rectTransform->SetGlobalMatrixToDraw(globalMat);
	glMultMatrixf(globalMat.Transposed().ptr());

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
	fontSrc = std::string(DEFAULT_FONT);
	exportTexPath = std::string(LIB_FONTS_PATH);
	scale = 1.0f;
	text = "Insert Text";
}
