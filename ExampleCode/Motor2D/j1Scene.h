#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "Animation.h"
struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* debug_tex;
	SDL_Texture* Map1;
	SDL_Texture* Background;
	SDL_Texture* Demo_ElementsAndCharacters_tex;

	Animation Character_Anim;
	Animation Character2_Anim;
	Animation Character3_Anim;

	iPoint Character1_Position;
	iPoint Character2_Position;
	iPoint Character3_Position;


	Animation Main_Scene;
	Animation* Current_Image=nullptr;

	SDL_Texture* TexT_Test=nullptr;
};

#endif // __j1SCENE_H__