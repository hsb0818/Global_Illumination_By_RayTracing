#ifndef _MAKE_PPM_H_
#define _MAKE_PPM_H_

#include <stdio.h>
#include <iostream>
#include "simple_gmath.h"

using namespace Simple_GMath;

FILE* CreatePPM(const char* pFileName, int width, int height, int max_level = 255)
{
	FILE* pFile;
	fopen_s(&pFile, pFileName, "wb");
	if (pFile == nullptr)
		return nullptr;

	fprintf(pFile, "P6\n");
	fprintf(pFile, "%d\n", width);
	fprintf(pFile, "%d\n", height);
	fprintf(pFile, "%d\n", 255);

	return pFile;
}

void DrawPPM(FILE* pFile, Color4 pixel)
{	
	unsigned char color[3];
	color[0] = (unsigned char)(pixel.r * 255.f);
	color[1] = (unsigned char)(pixel.g * 255.f);
	color[2] = (unsigned char)(pixel.b * 255.f);

	fwrite(color, sizeof(unsigned char), 3, pFile);
}

#endif