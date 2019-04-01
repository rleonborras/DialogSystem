#include "DialogSystem.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Window.h"
#include "j1Render.h"

bool DialogSystem::Awake(pugi::xml_node& conf) {

	bool ret = false;

	return true;
}


bool DialogSystem::Start() {

	bool ret = true;
	pugi::xml_document	DialogConfig_file;
	pugi::xml_node		Dialog_node;

	pugi::xml_parse_result result = DialogConfig_file.load_file("Dialog_Config.xml");

	if (result == NULL)
	{
		LOG("Could not load Dialog xml file. pugui error:%s", result.description());
		ret = false;
	}


	//SETUP THE CHARACTERS AND THE DIALOG CHART -> THE DIALOG DEPENDS ON THE SHAPE OF THE CHARTS
	createCharacters(DialogConfig_file);
	SetUpCharts(DialogConfig_file);

	//TODO 4 Create a new Dialog and Add it to the Main list
	dialogA = createDialog(DialogConfig_file, 0);
	Dialog_List.push_back(dialogA);

	counter = 0;

	return ret;
};


bool DialogSystem::Update(float dt) {

	if (currentDialog != nullptr && currentDialog->currentNode->dialogActive == true) {
		
			currentDialog->Input();
			currentDialog->Draw();
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		Influence_Level_Test = 60;
	}

	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN){
		CleanUp();
	}

	return true;
}


void Dialogue::Draw() {

	SDL_Rect temp;
	SDL_Texture* temp_tex;
	SDL_Texture* Dialog_Textures = App->dialog->returnDialog_Textures();
	Character temp_Character = App->dialog->getCharacter(currentNode->GetCharacterID());

	temp.x = temp.y = 0;
	int scale = App->dialog->scale;

	//WRITTING MACHINE LOOK
	App->dialog->counter++;
	if (App->dialog->counter % 2 == 0 && currentNode->dialogStop == false) {

		if (currentNode->lettercounter < currentNode->text.size()) {

			App->dialog->temp_text_print += currentNode->text.at(currentNode->lettercounter);
			currentNode->lettercounter++;
		}
		else
			currentNode->dialogEnded = true;
	}
	temp_tex = App->fonts->Print(App->dialog->temp_text_print.c_str(), temp.w, temp.h, 500);


	if (currentNode->dialogChart.y + currentNode->TextScrollPositon + temp.h >= App->dialog->SpawnDialog_Section.y + App->dialog->SpawnDialog_Section.h)
		currentNode->dialogStop = true;


	//RENDERING GENERIC DIALOG CHART
	SDL_RenderCopyEx(App->render->renderer,Dialog_Textures, &App->dialog->GeneraldialogChart, &currentNode->dialogChart, 0, NULL, SDL_FLIP_NONE);

	//SETTING UP THE DIALOG ZONE TO SPAWN
	if (currentNode->getPosition_Spawn() == TOP_RIGHT)
		App->dialog->SpawnDialog_Section.x = currentNode->dialogChart.x;
	else if(currentNode->getPosition_Spawn()== TOP_LEFT)
		App->dialog->SpawnDialog_Section.x = temp_Character.characterRect.w * scale + 15;

	App->dialog->SpawnDialog_Section.y = currentNode->dialogChart.y + App->dialog->YMargin;
	App->dialog->SpawnDialog_Section.h = 43 * scale;
	App->dialog->SpawnDialog_Section.w = 500;

	//RENDER TEXT
	App->render->SetViewPort(App->dialog->SpawnDialog_Section);
	App->render->Blit(temp_tex, 0, currentNode->TextScrollPositon, &temp);
	App->render->ResetViewPort();


	//RENDER OPTIONS TEXT
	std::list<DialogueOptions*>::iterator itemOptions = currentNode->dialogueOptions.begin();
	if (currentNode->dialogEnded == true && currentNode->dialogueOptions.size() > 1) {

		//RENDER OPTIONS CHART
		SDL_RenderCopyEx(App->render->renderer, Dialog_Textures, &App->dialog->GeneraldialogChart, &currentNode->optionsChart, 0, NULL, SDL_FLIP_NONE);
		App->render->Blit(Dialog_Textures, currentNode->optionsChart.x / scale - App->dialog->GeneraldialogChartEnd.w + 1, currentNode->optionsChart.y / scale, &App->dialog->GeneraldialogChartEnd, 1, scale, SDL_FLIP_HORIZONTAL);

		itemOptions = next(itemOptions);
		for (itemOptions; itemOptions != currentNode->dialogueOptions.end(); itemOptions++)
			App->render->Blit((*itemOptions)->optionText_tex, (*itemOptions)->optionTextPosition.x, (*itemOptions)->optionTextPosition.y, &(*itemOptions)->optionText_Rect);
		
		App->render->Blit(Dialog_Textures, currentNode->optionsChart.x / scale - App->dialog->GeneraldialogChartEnd.w + 1, currentNode->optionsChart.y / scale, &App->dialog->GeneraldialogChartEnd, 1, scale, SDL_FLIP_HORIZONTAL);
		currentNode->optionActive = true;
	}

	App->tex->UnLoad(temp_tex);
}


void  Dialogue::Input() {

	int x, y;
	App->input->GetMousePosition(x, y);

	std::list<DialogueOptions*>::iterator itemOptions = currentNode->dialogueOptions.begin();
	if (currentNode->dialogueOptions.size() > 1 && currentNode->dialogEnded == true) {
		for (; itemOptions != currentNode->dialogueOptions.end(); itemOptions = next(itemOptions)) {
			
			if (x > (*itemOptions)->optionTextPosition.x && x<(*itemOptions)->optionTextPosition.x + (*itemOptions)->optionText_Rect.w && y>(*itemOptions)->optionTextPosition.y && y < (*itemOptions)->optionTextPosition.y + (*itemOptions)->optionText_Rect.h) {
				if ((*itemOptions)->Minimum_Influence_Level < App->dialog->Influence_Level_Test) {

					if (App->input->GetMouseButtonDown(KEY_DOWN)) {
						
						if ((*itemOptions)->startAgain == true)
							checkOptions = (*itemOptions);
						else
							checkOptions = false;
						currentNode->optionActive = false;
						currentNode->resetNode();
						App->dialog->temp_text_print.clear();
						currentNode = (*itemOptions)->nextNode;
						currentNode->dialogActive = true;

						break;
					}
				}
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

		if (currentNode->dialogStop) {
			currentNode->TextScrollPositon -= App->dialog->SpawnDialog_Section.h;
			currentNode->dialogStop = false;
		}
		else if (currentNode->dialogEnded&&currentNode != nullptr&&currentNode->optionActive == false) {

			if (checkOptions != nullptr&&checkOptions->startAgain==true) 
				currentNode = dialogueNodes.front();
			
			App->dialog->temp_text_print.clear();
			currentNode->resetNode();
			DeactiveDialog();
		}
	}
}


bool DialogSystem::createNodes(pugi::xml_document& Dialog, Dialogue* Dialogue_list, int ID) {

	LOG("Create Existing Nodes");
	bool ret = true;

	pugi::xml_node		Dialog_node;
	treeDialogNode *tempNode = nullptr;

	//SETTING DIALOG NODES
	Dialog_node = Dialog.child("DialogSettup").child("Dialogs").child("Dialog");
	while (Dialog_node.attribute("ID").as_int() != ID)
		Dialog_node = Dialog_node.next_sibling();


	Dialog_node = Dialog_node.child("Node");
	for (; Dialog_node != nullptr; Dialog_node = Dialog_node.next_sibling()) {

		//TODO 5  Create a new treeDialogNode and fill its variables in order to define it, Remember the main functions as SetNodeID()
		tempNode = new treeDialogNode(Dialog_node.child("NPCTalk").child_value());
		tempNode->setNodeID(Dialog_node.attribute("ID").as_int());
		tempNode->setCharacterID(Dialog_node.attribute("CharacterID").as_int());
		tempNode->setPosition_Spawn(Dialog_node.attribute("NPC_Position").as_int());

		for (pugi::xml_node Options_node = Dialog_node.child("Option"); Options_node != nullptr; Options_node = Options_node.next_sibling()) {

			DialogueOptions *Options = new DialogueOptions(Options_node.child_value());
			Options->optionText_tex = App->fonts->Print(Options->text.c_str(), Options->optionText_Rect.w, Options->optionText_Rect.h);
			Options->NodeID = Options_node.attribute("ID").as_int();
			Options->nextNodeID = Options_node.attribute("NextNode").as_int();
			Options->Minimum_Influence_Level = Options_node.attribute("influenceLevel").as_int();
			Options->startAgain = Options_node.attribute("startAgain").as_bool();
			tempNode->dialogueOptions.push_back(Options);
		}

		tempNode->setUp_rects(500);
		Dialogue_list->dialogueNodes.push_back(tempNode);
	}

	//SETTING NEXTNODES OPTIONS
	std::list<treeDialogNode*>::iterator item = Dialogue_list->dialogueNodes.begin();
	for (; item != Dialogue_list->dialogueNodes.end(); item = next(item)) {

		std::list<DialogueOptions*>::iterator itemOptions = (*item)->dialogueOptions.begin();
		for (; itemOptions != (*item)->dialogueOptions.end(); itemOptions = next(itemOptions)) {


			for (std::list<treeDialogNode*>::iterator item2 = Dialogue_list->dialogueNodes.begin(); item2 != Dialogue_list->dialogueNodes.end(); item2 = next(item2)) {
				//TODO 6 Write the condition to explore the next node ID in order to point from the dialogOption to the nextNode
				if ((*itemOptions)->nextNodeID == (*item2)->GetNodeID())
					(*itemOptions)->nextNode = (*item2);
			}
		}
	}

	return true;
}


bool DialogSystem::createCharacters(pugi::xml_document& Dialog) {

	pugi::xml_node		Characters_node;
	bool ret = true;

	Characters_node = Dialog.child("DialogSettup").child("SettupOptions").child("Characters").child("Character");

	//SETTING CHARACTERS
	for (; Characters_node != nullptr; Characters_node = Characters_node.next_sibling()) {

		Character TempChar;

		TempChar.ID = Characters_node.attribute("ID").as_int();
		TempChar.characterName = Characters_node.attribute("name").as_string();
		TempChar.characterRect.x = Characters_node.attribute("x").as_int();
		TempChar.characterRect.y = Characters_node.attribute("y").as_int();
		TempChar.characterRect.w = Characters_node.attribute("w").as_int();
		TempChar.characterRect.h = Characters_node.attribute("h").as_int();
		Character_List.push_back(TempChar);
	}
	return true;
}


bool DialogSystem::SetUpCharts(pugi::xml_document& Dialog) {

	pugi::xml_node		Settings_Node;

	bool ret = true;

	Settings_Node = Dialog.child("DialogSettup").child("SettupOptions");
	DialogSpeed = Settings_Node.attribute("speed").as_float();
	scale = Settings_Node.attribute("scale").as_int();
	YMargin = Settings_Node.attribute("YMargin").as_int();
	Dialog_Textures = App->tex->Load(Settings_Node.child("DialogueChartTexture").attribute("path").as_string());
	Settings_Node = Settings_Node.child("DialogChart").child("MainPart");
	GeneraldialogChart.x = Settings_Node.attribute("x").as_int();
	GeneraldialogChart.y = Settings_Node.attribute("y").as_int();
	GeneraldialogChart.w = Settings_Node.attribute("w").as_int();
	GeneraldialogChart.h = Settings_Node.attribute("h").as_int();
	Settings_Node = Settings_Node.next_sibling();
	GeneraldialogChartEnd.x = Settings_Node.attribute("x").as_int();
	GeneraldialogChartEnd.y = Settings_Node.attribute("y").as_int();
	GeneraldialogChartEnd.w = Settings_Node.attribute("w").as_int();
	GeneraldialogChartEnd.h = Settings_Node.attribute("h").as_int();

	return true;
}


Dialogue* DialogSystem::createDialog(pugi::xml_document& Dialog, int ID) {

	pugi::xml_node		Dialog_Node;
	Dialog_Node = Dialog.child("DialogSettup").child("Dialogs");

	Dialogue *temp = nullptr;
	temp = new Dialogue(ID);

	createNodes(Dialog, temp, ID);
	temp->currentNode = temp->dialogueNodes.front();

	return temp;
}


bool treeDialogNode::setUp_rects(int lineMargin) {

	int scale = App->dialog->scale;

	SDL_Rect Temp_Rect;
	SDL_Texture* temp_tex;

	int height_comp = 0;
	int width_comp = 0;

	App->fonts->CalcSize(text.c_str(), width_comp, height_comp);

	if (width_comp >= lineMargin)
		temp_tex = App->fonts->Print(text.c_str(), Temp_Rect.w, Temp_Rect.h, lineMargin);
	else
		temp_tex = App->fonts->Print(text.c_str(), Temp_Rect.w, Temp_Rect.h, 0);

	Rect_Modifier = Temp_Rect.w + App->dialog->getCharacter(characterID).characterRect.w*scale;
	dialogChart = App->dialog->GeneraldialogChart;

	uint screen_Width;
	uint screen_Height;
	App->win->GetWindowSize(screen_Width, screen_Height);

	dialogChart.y = 60;
	dialogChart.h *= scale;
	dialogChart.w = Rect_Modifier;

	if (Position_Spawn == TOP_LEFT)
		dialogChart.x = 0;
	else if (Position_Spawn == TOP_RIGHT)
		dialogChart.x = screen_Width - dialogChart.w;

	int Options_Width_Check = 0;
	int PlaceOptions = (dialogChart.h / dialogueOptions.size()) - (dialogueOptions.back()->optionText_Rect.h / 2);

	std::list<DialogueOptions*>::iterator itemOptions = dialogueOptions.begin();
	itemOptions = next(itemOptions);//Remember that the 0 is when there is no option.

	for (; itemOptions != dialogueOptions.end(); itemOptions = next(itemOptions)) {

		if (Options_Width_Check <= (*itemOptions)->optionText_Rect.w)
			Options_Width_Check = (*itemOptions)->optionText_Rect.w;

		(*itemOptions)->optionTextPosition.y = PlaceOptions;
		PlaceOptions += (dialogChart.h / dialogueOptions.size());
	}

	optionsChart.w = Options_Width_Check + App->dialog->getCharacter(characterID).characterRect.w*scale;
	optionsChart.h = dialogChart.h;
	optionsChart.y = 500;
	optionsChart.x = screen_Width - optionsChart.w;

	itemOptions = dialogueOptions.begin();
	itemOptions = next(itemOptions);

	for (; itemOptions != dialogueOptions.end(); itemOptions = next(itemOptions)) {

		(*itemOptions)->optionTextPosition.x = optionsChart.x;
		(*itemOptions)->optionTextPosition.y += optionsChart.y;
	}

	App->tex->UnLoad(temp_tex);
	return true;
}


void treeDialogNode::cleanUP() {

	//TODO 7 Go through the list in order to call the cleanUp of the inner attribute.
	//std::list<DialogueOptions*>::iterator Dialog_Options = dialogueOptions.begin();
	//for (; Dialog_Options != dialogueOptions.end(); Dialog_Options = next(Dialog_Options)) {
	//	(*Dialog_Options)->CleanUp();
	//}
	dialogueOptions.clear();
}


void Dialogue::CleanUp() {

	//TODO 7 Go through the list in order to call the cleanUp of the inner attribute.
	//std::list<treeDialogNode*>::iterator Dialog_Nodes = dialogueNodes.begin();
	//for (; Dialog_Nodes != dialogueNodes.end(); Dialog_Nodes = next(Dialog_Nodes)) {
	//	(*Dialog_Nodes)->cleanUP();
	//}
	currentNode = nullptr;
	dialogueNodes.clear();
}


bool DialogSystem::CleanUp() {

	std::list<Dialogue*>::iterator Dialogs = Dialog_List.begin();
	for (; Dialogs != Dialog_List.end(); Dialogs = next(Dialogs)) {
		(*Dialogs)->CleanUp();
	}

	Dialog_List.clear();
	currentDialog = nullptr;
	App->tex->UnLoad(Dialog_Textures);

	return true;
};

DialogSystem::DialogSystem()
{  

}
