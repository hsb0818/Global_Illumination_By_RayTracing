#include <iostream>
#include "simple_gmath.h"
#include "ray.h"
#include "define_manager.h"
#include "scene_loader.h"
#include "define.h"

void InitApp()
{
	// MainCameraMng(); // need to creating the instance. this do it load_scene() function.
	LightMng();
	ObjectMng();
    
	Load_Scene("scene1-2.txt");
}

void FreeApp()
{
	LightManager::FreeInstance();
	ObjectManager::FreeInstance();
	MainCamera::FreeInstance();
}

int main(int argc, const char * argv[])
{
	InitApp();

	RayTrace("RayPPM.ppm", SCREEN_WIDTH, SCREEN_HEIGHT);

	FreeApp();

    return 0;
}

