#include "ComponentTextUI.h"
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
	

	static const float uvs[] = {
		0, 1,
		1, 1,
		1, 0,		
		0, 0
		
	};

	texCoords = new float2[4];
	memcpy(texCoords, uvs, sizeof(float2) * 4);

	rectTransform = myGO->GetComponentRectTransform();
		
	label.font = App->fontManager->GetFont(DEFAULT_FONT);
	
	//App->fontManager->LoadFont("federalescort.ttf", 20);
	SetText("Default Text");
	rectTransform->SetWidth((labelFrame[3].x - labelFrame[0].x), false);
	rectTransform->SetHeight((labelFrame[2].y - labelFrame[3].y), false);


	return true;
}

bool ComponentTextUI::Update()
{
	if (fadingOut) {
		FadeOut();
	}
	if (fadingIn) {
		FadeIn();
	}
	return true;
}



void ComponentTextUI::FadeIn()
{
	alpha += DELTA_ALPHA;
	if (alpha >= 1.0f) {
		fadingIn = false;
		alpha = 1.0f;
	}
}

void ComponentTextUI::FadeOut()
{
	alpha -= DELTA_ALPHA;
	if (alpha <= 0.0f) {
		fadingOut = false;
		alpha = 0.0f;
	}
}

void ComponentTextUI::CleanUp()
{
	CleanCharPlanes();
	rectTransform = nullptr;
	label.font = nullptr;
	myGO = nullptr;
	RELEASE_ARRAY(texCoords);
}

void ComponentTextUI::Load(Config * data)
{
	UUID = data->GetUInt("UUID");
	alpha = data->GetFloat("Alpha",1.0f );
	label.color = data->GetFloat3("Color", { 1,1,1 });
	SetFont(data->GetString("FontName", DEFAULT_FONT));
	SetFontScale(data->GetUInt("Scale", DEFAULT_SCALE));
	SetText(data->GetString("Txt", ""));
}

void ComponentTextUI::Save(Config & data) const
{
	data.AddUInt("UUID", UUID);
	data.AddFloat("Alpha", alpha);
	data.AddString("Txt", label.text.c_str());
	data.AddFloat3("Color", label.color);
	data.AddString("FontName", label.font->fontSrc.c_str());
	data.AddUInt("Scale", label.font->scale);
}

void ComponentTextUI::SetText(const char * txt)
{
	
		label.text = txt;
		CleanCharPlanes();

		FillCharPlanes();

		if (label.text.size() != 0) {//Adjust the container plane to the new text size 
			label.textOrigin.x = initOffsetX;
			EnframeLabel(labelFrame);
		}
	
}
void ComponentTextUI::SetAsCheckBox()
{
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
		float max_y = -100;

		int counter = 0;
		CharPlane* planeMaxY = nullptr;

		for (std::vector<CharPlane*>::iterator it = charPlanes.begin(); it != charPlanes.end(); it++, counter++)
		{
			float offsetY = offsetPlanes[counter].y;

			if (max_y < offsetY)
			{
				max_y = offsetY;
				planeMaxY = (*it);
			}

		}
		ret = { 0, planeMaxY->vertex[2].y + max_y + rectTransform->GetGlobalPos().y + label.textOrigin.y, 0 };
		break;
	}
	case 2: {//maxX		
		CharPlane* planeMaxX = nullptr;
		int counter = 0;
		float offset = -1;

		for (std::vector<CharPlane*>::iterator it = charPlanes.begin(); it != charPlanes.end(); it++, counter++)
		{
			if (counter == charPlanes.size() - 1)
			{
				planeMaxX = (*it);
				offset += offsetPlanes[counter].x;
				break;
			}

			offset += offsetPlanes[counter].x;
		}


		ret = { planeMaxX->vertex[2].x + offset + rectTransform->GetGlobalPos().x + label.textOrigin.x, 0, 0 };

		break;
	}
	case 3: {//minY	

		float minY = 100;
		CharPlane* planeMinY = nullptr;
		int counter = 0;

		for (std::vector<CharPlane*>::iterator it = charPlanes.begin(); it != charPlanes.end(); it++, counter++)
		{
			float offsetY = offsetPlanes[counter].y;

			if (minY > offsetY && offsetY != 0) //if plane_min_y == 0 it's an space
			{
				minY = offsetY;
				planeMinY = (*it);
			}
		}

		if (planeMinY != nullptr) {
			ret = { 0, planeMinY->vertex[0].y + minY + rectTransform->GetGlobalPos().y + label.textOrigin.y, 0 };
		}
		else {
			ret = { 0, 0 + minY + rectTransform->GetGlobalPos().y + label.textOrigin.y, 0 };
		}
		break;
	}
	}
	return ret;
}

void ComponentTextUI::AddCharPanel(char character, bool first)
{
	FT_Load_Char(label.font->face, (GLchar)character, FT_LOAD_RENDER);
	if (label.font->face->glyph == nullptr) { return; }
	float2 size = { (float)label.font->face->glyph->bitmap.width, (float)label.font->face->glyph->bitmap.rows };
	CharPlane* nwCharPlane = new CharPlane();	
	nwCharPlane->GenBuffer(size);
	if (first) { initOffsetX = size.x / 2; }
	if (character !=' ') {
		nwCharPlane->textureID = label.font->GetCharacterTexture(character); }
	charPlanes.push_back(nwCharPlane);
}

void ComponentTextUI::FillCharPlanes()
{
	bool first = false;
	for (int i = 0; i < label.text.size(); i++) {
		if (i == 0) { first = true; }
		AddCharPanel(label.text[i], first);
	}
	
	int counter = 0;
	offsetPlanes.clear();
	for (std::vector<CharPlane*>::const_iterator it = charPlanes.begin(); it != charPlanes.end(); it++, counter++)
	{
		float distancex = 0;
		float distancey = 0;
		
		Character* currChar = label.font->GetCharacter(label.text[counter]);
		Character* prevChar = nullptr;
		if (!currChar) { 
			currChar = label.font->GetCharacter(' ');
			
		}
		if (counter - 1 >= 0)
			prevChar = label.font->GetCharacter(label.text[counter - 1]);

		if (prevChar)
			distancex = prevChar->advance/ 2.f;

		distancex += currChar->advance/ 2.f;

		//Y offset
		float size = (float)currChar->size.y;
		float bearingy = (float)currChar->bearing.y;
		float center = (currChar->size.y /2);
		distancey = -(size - bearingy) + center;

		if (counter == 0)
		{
			distancex = 0;
		}

		offsetPlanes.push_back({ distancex, distancey, 0 });
	}


}

void ComponentTextUI::UpdateRectTransform()
{
	if(!charPlanes.empty())
		EnframeLabel(labelFrame);
}

void ComponentTextUI::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
	static const int maxSize = 32;
	if (ImGui::InputText("Label Text",(char*) label.text.c_str(), maxSize)){
		SetText(label.text.c_str());
	}
	if (ImGui::SliderFloat("Scale", &(label.font->scale), 8, MAX_CHARS,"%0.f")) {
		SetFontScale(label.font->scale);
	}
	ImGui::Checkbox("Draw Characters Frame", &drawCharPanel);
	ImGui::Checkbox("Draw Label Frame", &drawLabelrect);
	std::string currentFont = label.font->fontSrc;
	if (ImGui::BeginCombo("Fonts",currentFont.c_str() ))
	{
		std::vector<std::string> fonts = App->fontManager->singleFonts;

		for (int i = 0; i < fonts.size(); i++)
		{
			bool isSelected = false;
			
			if (strcmp(currentFont.c_str(), fonts[i].c_str()) == 0) {
				isSelected = true;
			}
			
			if (ImGui::Selectable(fonts[i].c_str(), isSelected)) {
				std::string newFont = std::string();
				App->fileSys->GetNameFromPath(fonts[i].c_str(), nullptr, nullptr, &newFont, nullptr);
				SetFont(newFont.c_str());

				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}

			}

		}
		ImGui::EndCombo();
		
	}
	ImGui::Spacing();
	ImGui::ColorPicker3("Color##2f", (float*)&label.color);
}

void ComponentTextUI::DrawUI()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	uint charNum = 0;
	int line = 0;
	uint lettersDrawn=0;
	float lineDistance = 0;	
	Character* firstChar = label.font->GetCharacter((GLchar)label.text[0]);
	
	float3 cursor = float3(label.textOrigin.x , label.textOrigin.y, 0);
	if (charPlanes.empty()) { 
		FillCharPlanes(); } 
	if (charPlanes.empty() || label.font->charsList.empty()) {
		return; }
	for (std::vector<CharPlane*>::iterator it = charPlanes.begin(); it != charPlanes.end(); it++, charNum++)
	{
		lettersDrawn++;
		float4x4 viewMatrix = float4x4::identity; // modelView Matrix
		float4x4 globalMatrix = float4x4::identity; // global Matrix ( from RectTransform)
		float4x4 incrementMatrix = float4x4::identity;// advance for every char Matrix ( from cursor)

		float3	pos = float3(rectTransform->GetGlobalPos().x, rectTransform->GetGlobalPos().y, 0);		
		globalMatrix.SetTranslatePart(pos); 	// set global Mat	

		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		viewMatrix.Set((float*)matrix);		// set MV mat

		
		Character* currChar = label.font->GetCharacter((GLchar)label.text[charNum]);
		Character* nextChar = nullptr;

		if (!currChar) {
			currChar = label.font->GetCharacter(' ');//if not avaliable glyph, put a space (like "í")
		}

		if (charNum < label.text.size() - 1 && label.text.size()>0)
			nextChar = label.font->GetCharacter((GLchar)label.text[charNum + 1]);
		else
			nextChar = label.font->GetCharacter((GLchar)"");
		if (lettersDrawn != 1) {
			cursor.x += offsetPlanes[charNum].x;
		}
		cursor.y = label.textOrigin.y + offsetPlanes[charNum].y + -line * lineSpacing;
				
		if (charNum == 0) // to start exacly where the component text rect is
		{
			lineDistance = currChar->size.x /1.5f;
		}
		else
			lineDistance += offsetPlanes[charNum].x;

		if (charNum == charPlanes.size() - 1)
			lineDistance += currChar->size.x / 1.5f;

		if ((lineDistance -10) > rectTransform->GetWidth()) //horizontal limit 
		{
			line++;
			cursor.x = label.textOrigin.x;
			cursor.y = offsetPlanes[charNum].y + label.textOrigin.y - (line * lineSpacing);
			lineDistance = 0;
		}
		if ((offsetPlanes[charNum].y + label.textOrigin.y)*(line+1) > rectTransform->GetHeight()) { //vertical limit 
			return;

		}

		incrementMatrix.SetTranslatePart(cursor); // set cursor mat

		if ((*it)->textureID != -1) {
			glColor4f(label.color.x, label.color.y, label.color.z, alpha);
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf((GLfloat*)(((globalMatrix * incrementMatrix).Transposed() * viewMatrix).v)); // multiplies all mat

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer
			glBindTexture(GL_TEXTURE_2D, 0);

			glLineWidth(4.0f);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0);

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

			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
						
			glColor3f(1.0f, 1.0f, 1.0f);

			
			if (drawCharPanel) {
				glBegin(GL_LINES);
				glColor3f(0.0f, 1.0f, 1.0f);

				glVertex3f((*it)->vertex[0].x, (*it)->vertex[0].y, (*it)->vertex[0].z);
				glVertex3f((*it)->vertex[1].x, (*it)->vertex[1].y, (*it)->vertex[1].z);

				glVertex3f((*it)->vertex[1].x, (*it)->vertex[1].y, (*it)->vertex[1].z);
				glVertex3f((*it)->vertex[2].x, (*it)->vertex[2].y, (*it)->vertex[2].z);

				glVertex3f((*it)->vertex[2].x, (*it)->vertex[2].y, (*it)->vertex[2].z);
				glVertex3f((*it)->vertex[3].x, (*it)->vertex[3].y, (*it)->vertex[3].z);

				glVertex3f((*it)->vertex[3].x, (*it)->vertex[3].y, (*it)->vertex[3].z);
				glVertex3f((*it)->vertex[0].x, (*it)->vertex[0].y, (*it)->vertex[0].z);

				glEnd();
			}

			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf((GLfloat*)viewMatrix.v);

			glColor3f(1.0f, 1.0f, 1.0f);

		}
		if (drawLabelrect) {
			glBegin(GL_LINES);
			glColor3f(0.0f, 0.0f, 1.0f);

			glVertex3f(labelFrame[0].x, labelFrame[0].y, labelFrame[0].z);
			glVertex3f(labelFrame[1].x, labelFrame[1].y, labelFrame[1].z);

			glVertex3f(labelFrame[1].x, labelFrame[1].y, labelFrame[1].z);
			glVertex3f(labelFrame[2].x, labelFrame[2].y, labelFrame[2].z);

			glVertex3f(labelFrame[2].x, labelFrame[2].y, labelFrame[2].z);
			glVertex3f(labelFrame[3].x, labelFrame[3].y, labelFrame[3].z);

			glVertex3f(labelFrame[3].x, labelFrame[3].y, labelFrame[3].z);
			glVertex3f(labelFrame[0].x, labelFrame[0].y, labelFrame[0].z);

			glEnd();
		}
		glColor3f(1.0f, 1.0f, 1.0f);
	}	
}

void ComponentTextUI::CharPlane::GenBuffer(float2 size)
{
	float2 halfSize = size / 2;
	
	const float vtx[] = { -halfSize.x ,-halfSize.y, 0,
								halfSize.x ,-halfSize.y, 0,
								halfSize.x, halfSize.y, 0,
								-halfSize.x, halfSize.y, 0 };
	vertex = new float3[4];
	memcpy(vertex, vtx, sizeof(float3) * 4);

	glGenBuffers(1, (GLuint*) &(vertexID));
	glBindBuffer(GL_ARRAY_BUFFER, vertexID); // set the type of buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 4, &vertex[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

void ComponentTextUI::ShiftNewLine(float3& cursor,int& current_line, int i)
{	
	current_line++;
	cursor.x = label.textOrigin.x;
	cursor.y = offsetPlanes[i].y + label.textOrigin.y -(current_line * lineSpacing);
	
}

void ComponentTextUI::SetFontScale(uint scale) {
	std::string name = label.font->fontSrc;
	//App->fontManager->RemoveFont(name.c_str());	
	label.font = App->fontManager->LoadFont(name.c_str(), scale);
	SetText(label.text.c_str());
	
}

void ComponentTextUI::SetFont(const char* font) {

	label.font = App->fontManager->GetFont(font);
	SetText(label.text.c_str());

}


void ComponentTextUI::CleanCharPlanes()
{
	for (int i = charPlanes.size()-1; i >=0; i--) {
		
		RELEASE_ARRAY(charPlanes[i]->vertex);
		glDeleteBuffers(1, &charPlanes[i]->vertexID);
		RELEASE(charPlanes[i]);
	}
	charPlanes.clear();
}
