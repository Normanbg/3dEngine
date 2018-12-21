#include "ComponentTextUI.h"
#include "ModuleFileSystem.h"
#include "TextureImporter.h"
#include "GameObject.h"
#include "ComponentRectTransform.h"

#include "mmgr/mmgr.h"
#include "FontManager.h"




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
	
	label.font = App->fontManager->LoadFont(DEFAULT_FONT, 10);
	SetText("Default Text.");
	//font.exportTexPath += (std::to_string(UUID) + PNG_FORMAT);
	//LoadLabel();
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

void ComponentTextUI::SetText(const char * txt)
{
	label.text = txt;
	charPlanes.clear();

	FillCharPlanes();

	if (label.text.size() != 0) //Adjust the container plane to the new text size 
		EnframeLabel(labelFrame);
}
void ComponentTextUI::EnframeLabel(float3 * points) {

	labelFrame[0] = GetCornerLabelPoint(0)+ GetCornerLabelPoint(3);//bottomleft;
	labelFrame[1] = GetCornerLabelPoint(0) + GetCornerLabelPoint(1);//topleft;
	labelFrame[2] = GetCornerLabelPoint(2) + GetCornerLabelPoint(1);//topright;
	labelFrame[3] = GetCornerLabelPoint(2) + GetCornerLabelPoint(3);//bottomrigth;
}
float3 ComponentTextUI::GetCornerLabelPoint(int corner) {
	float3 ret = float3(0, 0, 0);
	switch (corner) {
	case 0: {//minX		
		ret = { charPlanes[0]->vertex[0].x + rectTransform->GetGlobalPos().x + label.textOrigin.x,0,0 };
		break;
	}
	case 1: {//maxY		
		float max_y = 0;

		int counter = 0;
		CharPlane* max_y_plane = nullptr;

		for (std::vector<CharPlane*>::iterator it = charPlanes.begin(); it != charPlanes.end(); it++, counter++)
		{
			float plane_max_y = offsetPlanes[counter].y;

			if (max_y < plane_max_y)
			{
				max_y = plane_max_y;
				max_y_plane = (*it);
			}

		}
		ret = { 0, max_y_plane->vertex[0].y + max_y + rectTransform->GetGlobalPos().y + label.textOrigin.y, 0 };
		break;
	}
	case 2: {//maxX		
		//It will always lay on the last letter 
		CharPlane* last_img = nullptr;
		int counter = 0;
		float offset = -1;

		for (auto it = charPlanes.begin(); it != charPlanes.end(); it++, counter++) ///// -----------AUTO
		{
			if (counter == charPlanes.size() - 1)
			{
				last_img = (*it);
				offset += offsetPlanes[counter].x;
				break;
			}

			offset += offsetPlanes[counter].x;
		}


		ret = { last_img->vertex[2].x + offset + rectTransform->GetGlobalPos().x + label.textOrigin.x, 0, 0 };

		break;
	}
	case 3: {//minY	

		float min_y = 100000;
		CharPlane* min_y_plane = nullptr;
		int counter = 0;

		for (auto it = charPlanes.begin(); it != charPlanes.end(); it++, counter++)
		{
			float plane_min_y = offsetPlanes[counter].y;

			if (min_y > plane_min_y && plane_min_y != 0) //if plane_min_y == 0 it's an space
			{
				min_y = plane_min_y;
				min_y_plane = (*it);
			}
		}


		ret = { 0, min_y_plane->vertex[0].y + min_y + rectTransform->GetGlobalPos().y + label.textOrigin.y, 0 };

		break;
	}
	}
	return ret;
}

void ComponentTextUI::AddCharPanel(char character, float3 pos)
{
	FT_Load_Char(label.font->face, (GLchar)character, FT_LOAD_RENDER);
	float2 size = { (float)label.font->face->glyph->bitmap.width, (float)label.font->face->glyph->bitmap.rows };
	CharPlane* nwCharPlane = new CharPlane();
	nwCharPlane->GenBuffer(size);
	nwCharPlane->textureID = label.font->GetCharacterTexture(character);
	charPlanes.push_back(nwCharPlane);
}

void ComponentTextUI::FillCharPlanes()
{
	for (int i = 0; i < label.text.size(); i++)
		AddCharPanel(label.text[i], { 0,0,0 });

	int counter = 0;
	offsetPlanes.clear();
	for (std::vector<CharPlane*>::iterator it = charPlanes.begin(); it != charPlanes.end(); it++, counter++)
	{
		float distancex = 0;
		float distancey = 0;

		Character* curr_character = label.font->GetCharacter(label.text[counter]);
		Character* prev_character = nullptr;

		if (counter - 1 >= 0)
			prev_character = label.font->GetCharacter(label.text[counter - 1]);

		if (prev_character)
			distancex = prev_character->advance / 2.0;

		distancex += curr_character->advance / 2.0;

		//Y offset
		float size = (float)curr_character->size.y;
		float bearingy = (float)curr_character->bearing.y;
		float center_to_origin = (curr_character->size.y / 2);
		distancey = -(size - bearingy) + center_to_origin;

		if (counter == 0)
		{
			distancex = 0;
		}

		offsetPlanes.push_back({ distancex, distancey, 0 });
	}


}

/*void ComponentTextUI::LoadLabel(const char * _label, float _scale, const char * _font)
{
	//-------- Loading Font arial -->
	//font.ResetFont();	
	/*
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
		RELEASE_ARRAY(buffer);
		return;
	}

	int bm_w = 100*rectTransform->GetWidth(); // bitmap width 
	int bm_h = 100*rectTransform->GetHeight(); // bitmap height
	int line_h = 64; // line height 
	if (bm_w <= 0 || bm_h <= 0) {
		RELEASE_ARRAY(buffer);
		return;
	}
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
}*/



void ComponentTextUI::UpdateRectTransform()
{
	
	//LoadLabel(font.text.c_str(), font.scale, font.fontSrc.c_str());
}

void ComponentTextUI::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
	static const int maxSize = 16;
	if (ImGui::InputText("Label Text",(char*) label.text.c_str(), maxSize)){
		SetText(label.text.c_str());
	}
	if (ImGui::SliderInt("Scale", &label.scale, 10, 30)) {
		
	}
	/*if (ImGui::BeginCombo("Fonts", label.fontSrc.c_str()))
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
				//LoadLabel(font.text.c_str(), font.scale, font.fontSrc.c_str());

				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}

			}

		}
		ImGui::EndCombo();
	}*/
}

void ComponentTextUI::DrawUI()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	bool draw_section = false;
	uint counter = 0;
	uint line = 0;
	uint lettersDrawn=0;
	float lineDistance = 0;	
	float initOffset = label.textOrigin.x;
	float3 cursor = float3(label.textOrigin.x, label.textOrigin.y, 0);

	for (std::vector<CharPlane*>::iterator it = charPlanes.begin(); it != charPlanes.end(); it++, counter++)
	{
		if (!draw_section) {// && (counter < section.x || counter > section.y))

			lettersDrawn++;
			
			//ComponentTransform* trans = rectTrans->GetTransform();

			// Get Parent Matrix
			float4x4 view_mat = float4x4::identity;

			GLfloat matrix[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
			view_mat.Set((float*)matrix);

			// Create increment matrix
			float4x4 increment = float4x4::identity;

			Character* currChar = label.font->GetCharacter((GLchar)label.text[counter]);
			Character* nextChar = nullptr;

			if (counter < label.text.size() - 1)
				nextChar = label.font->GetCharacter((GLchar)label.text[counter + 1]);
			else
				nextChar = label.font->GetCharacter((GLchar)"");

			cursor.x += offsetPlanes[counter].x;
			cursor.y = label.textOrigin.y + offsetPlanes[counter].y + -line * lineSpacing;

			if (draw_section && lettersDrawn == 1)
				cursor.x -= offsetPlanes[counter].x;

			if (counter == 0)
			{
				lineDistance = currChar->size.x / 2.0f;
			}
			else
				lineDistance += offsetPlanes[counter].x;

			if (counter == charPlanes.size() - 1)
				lineDistance += currChar->size.x / 2.0f;

			if (lineDistance > rectTransform->GetWidth())
			{
				/*if (ControlNewLine(cursor, offsetPlanes, cmp_container->GetClipping(), current_line, counter, text_origin) == false)
					return;

				lineDistance = 0;*/
			}
			/**/
			glPushMatrix();
			float4x4 globalMat;
			rectTransform->SetGlobalMatrixToDraw(globalMat);
			glMultMatrixf(globalMat.Transposed().ptr());

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer
			glBindTexture(GL_TEXTURE_2D, 0);

			glLineWidth(4.0f);
						
			glBindTexture(GL_TEXTURE_2D, (*it)->textureID);
			glTexCoordPointer(2, GL_FLOAT, 0, &(texCoords[0]));
			
			glBindBuffer(GL_ARRAY_BUFFER, (*it)->vertexID);
			glVertexPointer(3, GL_FLOAT, 0, NULL);
			glDrawArrays(GL_QUADS, 0, 4);

			glLineWidth(1.0f);

			glBindTexture(GL_TEXTURE_2D, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			glPopMatrix(); 
/*
			glBegin(GL_LINES);
			glColor3f(0.0f, 1.0f, 0.0f);

			glVertex3f((*it)->vertex[0].x, (*it)->vertex[0].y, (*it)->vertex[0].z);
			glVertex3f((*it)->vertex[1].x, (*it)->vertex[1].y, (*it)->vertex[1].z);

			glVertex3f((*it)->vertex[1].x, (*it)->vertex[1].y, (*it)->vertex[1].z);
			glVertex3f((*it)->vertex[2].x, (*it)->vertex[2].y, (*it)->vertex[2].z);

			glVertex3f((*it)->vertex[2].x, (*it)->vertex[2].y, (*it)->vertex[2].z);
			glVertex3f((*it)->vertex[3].x, (*it)->vertex[3].y, (*it)->vertex[3].z);

			glVertex3f((*it)->vertex[3].x, (*it)->vertex[3].y, (*it)->vertex[3].z);
			glVertex3f((*it)->vertex[0].x, (*it)->vertex[0].y, (*it)->vertex[0].z);

			glEnd();
			
			increment.SetTranslatePart(cursor);

			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf((GLfloat*)(((trans->GetGlobalViewMatrix() * increment).Transposed() * view_mat).v));

			glBindBuffer(GL_ARRAY_BUFFER, (*it)->GetPlane()->GetMesh()->vertices_id);
			glVertexPointer(3, GL_FLOAT, 0, NULL);

			glBindBuffer(GL_ARRAY_BUFFER, (*it)->GetPlane()->GetMesh()->uvs_id);
			glBindTexture(GL_TEXTURE_2D, (*it)->GetImgID());
			glTexCoordPointer(3, GL_FLOAT, 0, NULL);

			glColor3f(color.x, color.y, color.z);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*it)->GetPlane()->GetMesh()->indices_id);

			glDrawElements(GL_TRIANGLES, (*it)->GetPlane()->GetMesh()->num_indices, GL_UNSIGNED_INT, NULL);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf((GLfloat*)view_mat.v);*/
		}
	}
	/*
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
	glPopMatrix();*/


	/*
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	
	glVertex3f(labelFrame[0].x, labelFrame[0].y, labelFrame[0].z);
	glVertex3f(labelFrame[1].x, labelFrame[1].y, labelFrame[1].z);

	glVertex3f(labelFrame[1].x, labelFrame[1].y, labelFrame[1].z);
	glVertex3f(labelFrame[2].x, labelFrame[2].y, labelFrame[2].z);

	glVertex3f(labelFrame[2].x, labelFrame[2].y, labelFrame[2].z);
	glVertex3f(labelFrame[3].x, labelFrame[3].y, labelFrame[3].z);

	glVertex3f(labelFrame[3].x, labelFrame[3].y, labelFrame[3].z);
	glVertex3f(labelFrame[0].x, labelFrame[0].y, labelFrame[0].z);

	glEnd();*/
}

void ComponentTextUI::CharPlane::GenBuffer(float2 size)
{
	static const float vtx[] = { -1,-1, 0,
								  -1,1, 0,
								  1,1, 0,
								   1,-1, 0 };
	/*static const float vtx[] = { -size.x/2 ,size.y/2, 0,
								size.x/2 ,size.y/2, 0,
								-size.x/2, -size.y/2, 0, 
								size.x/2, -size.y/2, 0 };*/
	vertex = new float3[4];
	memcpy(vertex, vtx, sizeof(float3) * 4);

	glGenBuffers(1, (GLuint*) &(vertexID));
	glBindBuffer(GL_ARRAY_BUFFER, vertexID); // set the type of buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 4, &vertex[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
