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
	Demo_ElementsAndCharacters_tex = App->tex->Load("textures/Elements_To_Demo.png");
	
	Character_Anim.PushBack({ 61,84,16,31 });
	Character_Anim.PushBack({ 77,84,16,31 });
	Character_Anim.PushBack({ 93,84,16,31 });
	Character_Anim.PushBack({ 77,84,16,31 });
	Character_Anim.speed = 0.08;
	Character_Anim.loop = true;


	Character2_Anim.PushBack({ 77,115,16,27 });
	Character2_Anim.PushBack({ 93,115,16,27 });
	Character2_Anim.PushBack({ 77,115,16,27 });
	Character2_Anim.PushBack({ 61,115,16,27 });
	Character2_Anim.speed = 0.07;
	Character2_Anim.loop = true;

	Character3_Anim.PushBack({ 77,57,16,27 });
	Character3_Anim.PushBack({ 93,57,16,27 });
	Character3_Anim.PushBack({ 77,57,16,27 });
	Character3_Anim.PushBack({ 61,57,16,27 });
	Character3_Anim.speed = 0.08;
	Character3_Anim.loop = true;

	Map1 = App->tex->Load("textures/Scenario_1.png");
	Main_Scene.PushBack({ 0, 0, 417, 344 });
	Main_Scene.PushBack({ 417, 0, 417, 344 });
	Main_Scene.PushBack({ 0, 344, 417, 344 });
	Main_Scene.PushBack({ 417,344, 417, 344 });
	Main_Scene.loop = true;
	Main_Scene.speed = 0.05;

	Current_Image = &Main_Scene;
	debug_tex = App->tex->Load("maps/path2.png");

	//TexT_Test = App->fonts->Print("Hello");

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
	int scale = 2;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 2;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 2;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 2;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 2;

	App->render->Blit(Background, 0, 0, NULL, false);
	App->render->Blit(Map1, 45, 20, &Current_Image->GetCurrentFrame(dt), 1, 2);
	App->render->Blit(Demo_ElementsAndCharacters_tex, 305, 155, &Character_Anim.GetCurrentFrame(dt), 1, scale);
	App->render->Blit(Demo_ElementsAndCharacters_tex, 117, 205, &Character2_Anim.GetCurrentFrame(dt), 1, scale);
	App->render->Blit(Demo_ElementsAndCharacters_tex, 230, 260, &Character3_Anim.GetCurrentFrame(dt), 1, scale);

	Character1_Position.x = 305*scale;
	Character1_Position.y = 155 * scale;

	Character2_Position.x = 117 * scale;
	Character2_Position.y = 205 * scale;

	Character3_Position.x = 230 * scale;
	Character3_Position.y = 260 * scale;

	//if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
	//	App->dialog->activeDialog();
	int x, y;
	App->input->GetMousePosition(x, y);

	if (x > Character2_Position.x&&x<Character2_Position.x + Character2_Anim.GetCurrentFrame(dt).w*scale && y>Character2_Position.y &&y < Character2_Position.y + Character2_Anim.GetCurrentFrame(dt).y*scale)
		if (App->input->GetMouseButtonDown(KEY_DOWN)) 
			App->dialog->StartDialogEvent(App->dialog->dialogB);

	if (x > Character3_Position.x&&x<Character3_Position.x + Character3_Anim.GetCurrentFrame(dt).w*scale && y>Character3_Position.y &&y < Character3_Position.y + Character3_Anim.GetCurrentFrame(dt).y*scale)
		if (App->input->GetMouseButtonDown(KEY_DOWN))
			App->dialog->StartDialogEvent(App->dialog->dialogA);


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
