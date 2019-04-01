#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Fonts.h"
#include "j1Scene.h"
#include "DialogSystem.h"

j1Scene::j1Scene() : j1Module()
{
	name.assign("scene");
}


// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;
	
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{

	Background = App->tex->Load("textures/Background.png");
	Character1 = { 78,0,34,54 };
	Character1_Position.x = 320 ;
	Character1_Position.y = 320 ;
	//TexT_Test = App->fonts->Print("Hello");

	Demo_ElementsAndCharacters_tex = App->tex->Load("textures/Demo_Chart.png");
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	int scale = 3;

	App->render->Blit(Background, 0, 0, NULL, false);

	App->render->Blit(Demo_ElementsAndCharacters_tex, Character1_Position.x, Character1_Position.y, &Character1, 1, scale);

	int x, y;
	App->input->GetMousePosition(x, y);

	if (x > Character1_Position.x&&x<Character1_Position.x + Character1.w*scale && y>Character1_Position.y &&y < Character1_Position.y + Character1.h*scale)
		if (App->input->GetMouseButtonDown(KEY_DOWN)) {
			//TODO 8 start Your dialog! 


		}


	return true;
}
  
// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
