#ifndef RayTracing_MacOS_scene_loader_h
#define RayTracing_MacOS_scene_loader_h

#include <iostream>
#include <stdio.h>
#include <fstream>
#include "simple_gmath.h"
#include "define_manager.h"
#include "define.h"

using namespace Simple_GMath;


char JumpNewLine(FILE* pFile)
{
	char c = '\0';

	do
	{			
		if (feof(pFile))
			break;

		fread(&c, sizeof(char), 1, pFile);

	} while ((c == '\n') || (c == '\r'));
	
	fseek(pFile, -(int)(sizeof(char)), SEEK_CUR);

	return c;
}

void Load_Light(FILE* pFile, char type);
void Load_Object(FILE* pFile, char type);

bool Load_Scene(const char* pFileName)
{
    FILE* pFile;
	fopen_s(&pFile, pFileName, "r");
	if (pFile == nullptr)
		return false;

    Point3 eye;
    Point3 look;
    Vector3 up;
    float fovx = 0.f;
    
    fscanf_s(pFile, "%f %f %f", &eye.x, &eye.y, &eye.z);
    fscanf_s(pFile, "%f %f %f", &up.x, &up.y, &up.z);
    fscanf_s(pFile, "%f %f %f", &look.x, &look.y, &look.z);
    fscanf_s(pFile, "%f", &fovx);

	MainCamera::CreateInstance(eye, look, up, fovx, SCREEN_WIDTH, SCREEN_HEIGHT);

	int light_count = 0;
	fscanf_s(pFile, "%d", &light_count);

	JumpNewLine(pFile);

	for (int i=0; i<light_count; ++i)
	{
		char type;
		fread(&type, sizeof(char), 1, pFile);
		Load_Light(pFile, type);
	}
	printf("total %d lights are loaded\n", light_count);

	int object_count = 0;
	fscanf_s(pFile, "%d", &object_count);

	JumpNewLine(pFile);

	for (int i=0; i<object_count; ++i)
	{
		char type;
		fread(&type, sizeof(char), 1, pFile);
		Load_Object(pFile, type);
	}
	printf("total %d objects are loaded\n", object_count);

    fclose(pFile);
    return true;
}

void Load_Object(FILE* pFile, char type)
{
	Color4 diffuse;
	Color4 specular;
	float shininess = 0.f;
	float reflected_coef = 0.f;
	float transmitted_coef = 0.f;
	float refracted_index = 0.f;

	switch (type)
	{
	case 'c':
	case 's':
	case 'p':
	case 'o':
		{
			fscanf_s(pFile, "%f %f %f", &diffuse.r, &diffuse.g, &diffuse.b);
			fscanf_s(pFile, "%f %f %f", &specular.r, &specular.g, &specular.b);
			fscanf_s(pFile, "%f", &shininess);
			fscanf_s(pFile, "%f", &reflected_coef);
			fscanf_s(pFile, "%f", &transmitted_coef);
			fscanf_s(pFile, "%f", &refracted_index);

			diffuse /= 255.f;
			specular /= 255.f;

			break;
		}
	}

	switch (type)
	{
	case 'c':
		{	
			Point3 points[8];
			for (int i=0; i<8; ++i)
				fscanf_s(pFile, "%f %f %f", &points[i].x, &points[i].y, &points[i].z);

			ObjectMng().Insert(new Cube(diffuse, specular, shininess, reflected_coef,
				transmitted_coef, refracted_index, points, 8));
			break;
		}
	case 's':
		{
			Point3 center;
			float radius = 0.f;
			fscanf_s(pFile, "%f %f %f", &center.x, &center.y, &center.z);
			fscanf_s(pFile, "%f", &radius);

			ObjectMng().Insert(new Sphere(diffuse, specular, shininess, reflected_coef,
				transmitted_coef, refracted_index, center, radius));
			break;
		}
	case 'p':
		{
			Vector3 normal;
			Point3 point_on_plane;
			fscanf_s(pFile, "%f %f %f", &normal.x, &normal.y, &normal.z);
			fscanf_s(pFile, "%f %f %f", &point_on_plane.x, &point_on_plane.y, &point_on_plane.z);

			ObjectMng().Insert(new Plane(diffuse, specular, shininess, reflected_coef,
				transmitted_coef, refracted_index, point_on_plane, normal.Normalize()));
			break;
		}
	case 'o':
		{
			JumpNewLine(pFile);
			char filename[_MAX_PATH] = "";
			fgets(filename, _MAX_PATH, pFile);	

			ModelObj* pNew_Obj = new ModelObj(filename);
			pNew_Obj->Set(diffuse, specular, shininess, reflected_coef,
				transmitted_coef, refracted_index);

			ObjectMng().Insert(pNew_Obj);

			break;
		}
	}

	JumpNewLine(pFile);
}

void Load_Light(FILE* pFile, char type)
{
	switch (type)
	{
	case 'p':
		{
			Point3 position;
			Color4 color;
			fscanf_s(pFile, "%f %f %f", &position.x, &position.y, &position.z);
			fscanf_s(pFile, "%f %f %f", &color.r, &color.g, &color.b);

			color /= 255.f;

			LightMng().Insert(new PointLight(position, color));
			break;
		}
	case 'd':
		{
			Vector3 direction;
			Color4 color;
			fscanf_s(pFile, "%f %f %f", &direction.x, &direction.y, &direction.z);
			fscanf_s(pFile, "%f %f %f", &color.r, &color.g, &color.b);

			color /= 255.f;

			LightMng().Insert(new DirectLight(direction, color));
			break;
		}
	}

	JumpNewLine(pFile);
}

#endif
