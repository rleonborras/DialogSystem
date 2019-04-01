#ifndef DIALOG_SYSTEM_CLASS
#define DIALOG_SYSTEM_CLASS

#include <iostream> 
#include <sstream>
#include <vector>
#include "j1App.h"
#include "j1Module.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "SDL/include/SDL.h"

class treeDialogNode;

enum dialogSpawn{
	TOP_LEFT=0,
	TOP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT,
	NONE_DIRECTION
};


struct Character {
	SDL_Rect characterRect;
	int ID;
	std::string characterName;
};


class DialogueOptions
{
public:

	DialogueOptions(std::string Text) :text(Text) {};
	DialogueOptions(std::string Text, treeDialogNode* NextNode) :text(Text), nextNode(NextNode) {};

	void CleanUp() {
		App->tex->UnLoad(optionText_tex);
		nextNode=nullptr;
	}

	std::string text;

	SDL_Texture* optionText_tex;
	SDL_Rect optionText_Rect;

	iPoint optionTextPosition;

	int nextNodeID;
	int NodeID;
	int Minimum_Influence_Level;

	bool startAgain;

	treeDialogNode* nextNode;
};


class treeDialogNode 
{
public:

	treeDialogNode(std::string Text): text(Text) {}

	void cleanUP();

	bool setUp_rects(int lineMargin);

	int GetCharacterID() {
		return characterID;
	};

	void setCharacterID(int CharacterID) {
		characterID = CharacterID;
	}

	int GetOptionCharacterID() {
		return OptionCharacterID;
	}
	void setOptionCharacterID(int CharacterID) {
		OptionCharacterID = CharacterID;
	}

	int GetNodeID() const{
		return nodeID;
	}
	void setNodeID(int node_ID) {
		nodeID = node_ID;
	}
	void setPosition_Spawn(int Position) {
		Position_Spawn=(dialogSpawn)Position;
	}
	dialogSpawn getPosition_Spawn()const {
		return Position_Spawn;
	}
	void resetNode() {
		dialogActive = false;
		lettercounter = 0;
		TextScrollPositon = 0;
		dialogEnded = false;
		dialogStop = false;
	};


public:

	SDL_Rect dialogChart;
	SDL_Rect optionsChart;

	bool dialogActive = false;
	bool dialogEnded = false;
	bool dialogStop = false;
	bool optionActive = false;

	int lettercounter = 0;
	int TextScrollPositon;

	std::string text;
	std::list <DialogueOptions*> dialogueOptions;

private:

	int Rect_Modifier;
	int nodeID;
	int characterID;
	int OptionCharacterID;
	dialogSpawn Position_Spawn;

};


class Dialogue {
public:
	
	Dialogue(int DialogID) :DialogID(DialogID) {};

	void Input();

	void Draw();

	void CleanUp();

	void ActiveDialog() {
		DialogueActive = true;
		currentNode->dialogActive = true;
	}
	void DeactiveDialog() {
		DialogueActive = false;
		currentNode->resetNode();
	}
	bool getDialogStatus()const {
		return DialogueActive;
	}

public:

	int DialogID;

	treeDialogNode * currentNode = nullptr;
	std::list <treeDialogNode*> dialogueNodes;

private:

	bool DialogueActive = false;
	DialogueOptions* checkOptions=nullptr;

};



class DialogSystem:public j1Module
{
public:
	DialogSystem();
	~DialogSystem() {};

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	//Creates the nodes of the dialogs
	bool createNodes(pugi::xml_document &Dialog, Dialogue* Dialogue_list,int ID);
	//Creates the characters
	bool createCharacters(pugi::xml_document &Dialog);
	//Creates the DialogCharts of each Node
	bool SetUpCharts(pugi::xml_document &Dialog);

	Dialogue* createDialog(pugi::xml_document& Dialog, int ID);
	
	Character getCharacter(int ID) const {
		return Character_List.at(ID);
	}

	void StartDialogEvent(Dialogue* Dialog) {
		if (currentDialog == nullptr || currentDialog->getDialogStatus() == false) {
			temp_text_print.clear();
			currentDialog = Dialog;
			currentDialog->ActiveDialog();
		}
	}
	SDL_Texture* returnDialog_Textures() {
		return Dialog_Textures;
	}
	//TODO 4 Create a new Dialog and Add it to the Main list
	Dialogue*dialogA = nullptr;



public:

	int YMargin;
	int counter = 0;
	int Influence_Level_Test = 20;

	float DialogSpeed;
	int scale;

	SDL_Rect GeneraldialogChart;
	SDL_Rect GeneraldialogChartEnd;
	SDL_Rect SpawnDialog_Section;

	std::string temp_text_print;

private:

	Dialogue * currentDialog = nullptr;

	std::vector<Character>Character_List;

	//TODO 4 Create the Main list here
	std::list<Dialogue*> Dialog_List;

	SDL_Texture* Dialog_Textures = nullptr;
};

#endif // !