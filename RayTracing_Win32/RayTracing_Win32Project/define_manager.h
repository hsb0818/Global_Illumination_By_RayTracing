#ifndef __DEFINE_MANAGER_H__
#define __DEFINE_MANAGER_H__

#include "object.h"
#include "light.h"
#include "Camera.h"


static LightManager& LightMng()
{
    return (*LightManager::GetInstance());
}

static ObjectManager& ObjectMng()
{
	return *ObjectManager::GetInstance();
}

static MainCamera& MainCameraMng()
{
    return *MainCamera::GetInstance();
}

#endif