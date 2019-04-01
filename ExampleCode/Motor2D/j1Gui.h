
#ifndef __j1GUI_H__
#define __j1GUI_H__
#define _CRT_SECURE_NO_WARNINGS


#include "j1Module.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Fonts.h"
#include "j1Audio.h"
#include "DialogSystem.h"


#define CURSOR_WIDTH 2

enum UI_Type {

	LABEL,
	PANEL,
	NON_INTERACTIVE,
	NONE_TYPE
};
enum Button_Logic {
	NONE_LOGIC
};

class UI_Element {

public:

	UI_Element() {};
	UI_Element(UI_Type type, iPoint position, SDL_Rect Image_Rect, Button_Logic logic, UI_Element* Parent = nullptr)
		:type(type), UI_Rect(Image_Rect), Position(position), initialPosition(position), Logic(logic), Parent(Parent) {
	};
public:

	virtual void Update(){
	}

	void Draw();

public:


	iPoint initialPosition;
	iPoint Position;

	UI_Type type;
	Button_Logic Logic;

	const char* text = nullptr;
	SDL_Rect UI_Rect;
	SDL_Texture* texture;

	std::list<UI_Element*> Child_List;
	UI_Element* Parent=nullptr;
	float scale = 1.0f;
	float angle = 0.0f;
	bool isActive = true;
};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update(float dt);
	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	SDL_Texture* GetAtlas() const;

	iPoint lastMousePos = { 0,0 };
	iPoint newMousePos = { 0,0 };

	UI_Element* Add_UIElement(UI_Type type, iPoint position, SDL_Rect Image_Rect, Button_Logic = NONE_LOGIC, float scale = 1.0f, SDL_Color Color = { 255,255,255,255 }, UI_Element* Parent = nullptr, const char* Text = nullptr);

	std::list <UI_Element*> UI_Elements_List;

	bool Debug = false;
	DialogSystem MainScreen;

private:


	SDL_Texture* atlas;
	std::string atlas_file_name;
};

#endif // __j1GUI_H__