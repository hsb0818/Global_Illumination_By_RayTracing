//
//  light.cpp
//  RayTracing
//
//  Created by SungBinHong on 2014. 6. 19..
//  Copyright (c) 2014 years Teraphonia. All rights reserved.
//

#include "light.h"

LightManager* LightManager::m_pInstance = nullptr;

void LightManager::Free()
{
	LIST::iterator itr = list.begin();
	for (; itr!=list.end(); ++itr)
	{
		if ((*itr) == nullptr)
			continue;

		delete (*itr);
	}
	list.clear();

	LIST(list).swap(list); // for reset the memory space. this is a simple skill. :)
}