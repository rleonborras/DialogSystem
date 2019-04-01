#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.assign("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	
	//atlas = App->tex->Load();

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{

	return true;
}
bool j1Gui::Update(float dt) {

	return true;
}
// Called after all Updates
bool j1Gui::PostUpdate()
{

	std::list<UI_Element*>::iterator Item = UI_Elements_List.begin();
	for (; Item != UI_Elements_List.end(); Item = next(Item)) {
		Item._Ptr->_Myval->Update();
		if (Item._Ptr->_Myval->isActive) {
			Item._Ptr->_Myval->Draw();
		}
	}
	
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

UI_Element* j1Gui::Add_UIElement(UI_Type type, iPoint position, SDL_Rect Image_Rect, Button_Logic ,
	float scale, SDL_Color Color,UI_Element* Parent, const char* Text ) {

	UI_Element* ret = nullptr;
	ret->scale = scale;
	ret->isActive = true;
	return ret;

}

void UI_Element::Draw() {
	
	//App->render->Blit(texture, Position.x, Position.y,&UI_Rect, false, angle, 0, 0, SDL_FLIP_NONE);
	
}
