#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>
#include <math.h>
#include <gl/glut.h>
#include "obj_parser.h"
#include "StringTokenizer.h"

using namespace std;

const string split_sign		= " ";
const string split_sign1	= "/";
const string split_sign2	= "//";
void DeleteOption(string& line) { line = &line[2]; }

template <typename T>
T ProcessData_3D(CStringTokenizer& token)
{
	float v[3] = {0, };
	for (int i=0; i<3; ++i)
		v[i] = atof(token.nextToken().c_str());

	return T(v[0], v[1], v[2]);
}

/*
 *	v//vn or
 *	v1/vt/vn
*/
Face ProcessFace(CStringTokenizer& token);

/*
 *	line[0] == 'v' && line[1] == 't'
 *	this code is better than the code line.substr(0, 2) == "v "
 *	because of there is no line or just one character,
 *	the second way is throwing the error.
 *	but the first way will check left, then right.
 *	so, fine if there is not exist line[1].
 */
bool ObjData::Load(const char* pFileName)
{
	ifstream readStream(pFileName, std::ios::in);
	if (readStream.is_open() == false)
		return false;

	readStream.seekg(0, std::ios::end);
	streamoff filesize  = readStream.tellg();
	readStream.seekg(0, std::ios::beg);
	streamoff readData = 0;

	m_fileSize = filesize;

	string line;
	while (getline(readStream, line))
	{
		if (line.empty())
			continue;

		if (line[0] == 'v' && line[1] == ' ')
			ProcessData(OBJ_OPTION::V, line);
		if (line[0] == 'v' && line[1] == 't')
			ProcessData(OBJ_OPTION::VT, line);
		if (line[0] == 'v' && line[1] == 'n')
			ProcessData(OBJ_OPTION::VN, line);
		if (line[0] == 'f' && line[1] == ' ')
			ProcessData(OBJ_OPTION::F, line);

		readData = readStream.tellg();
		if (readData > 0)
		{
			printf("\r");
			cout<< "per : "<< ((float)readData / (float)filesize) * 100.f;
		}
	}
	printf("\n");
	readStream.close();
	isLoaded = true;
	return true;
}

void ObjData::ProcessData(OBJ_OPTION option, string& line)
{
	DeleteOption(line);
	CStringTokenizer token(line, split_sign);

	switch (option)
	{
	case OBJ_OPTION::V:
		{
			Point3 temp_p = ProcessData_3D<Point3>(token);
			temp_p.z -= 13.f;
			m_vertices.push_back(temp_p);
			break;
		}
	case OBJ_OPTION::VT:
		m_vertices_texture.push_back(ProcessData_3D<Point3>(token));
		break;
	case OBJ_OPTION::VN:
		m_normals.push_back(ProcessData_3D<Vector3>(token));
		break;
	case OBJ_OPTION::F:
		m_faces.push_back(ProcessFace(token));
		break;
	}
}

/*		Face& face = m_faces[i];
		Vector3* n[3] = { GetNormal(face.normal_idx[0]),
			GetNormal(face.normal_idx[1]),
			GetNormal(face.normal_idx[2])
		};
		Point3* v[3] = { GetVertex(face.vertex_idx[0]),
			GetVertex(face.vertex_idx[1]),
			GetVertex(face.vertex_idx[2])
		};
		
		for (int j=0; j<3; ++j)
		{
			double dn[3] = {n[j]->x, n[j]->y, n[j]->z};
			double dv[3] = {v[j]->x, v[j]->y, v[j]->z};
			glNormal3dv(dn);
			glVertex3dv(dv);
		}
*/
void ObjData::Display()
{
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < GetNumberOfFaces(); i++) 
	{			
		Face& face = m_faces[i];

		for (int j=0; j<3; ++j)
		{
			Point3& v = m_vertices[face.vertex_idx[j]];
			Vector3& n = m_normals[face.normal_idx[j]];
			glColor3f(0.f, 1.f, 0.f);
			glNormal3f(n.x, n.y, n.z);
			glVertex3f(v.x, v.y, v.z);
		}
	}
	glEnd();
}

Vector3* ObjData::GetNormal(const int idx)
{
	assert(idx >= 0 && idx < (int)m_normals.size());
	return &m_normals[idx];
}

Point3* ObjData::GetVertex(const int idx)
{
	assert(idx >= 0 && idx < (int)m_vertices.size());
	return &m_vertices[idx];
}

Face ProcessFace(CStringTokenizer& token)
{
	int vertex_idx[3] = {0, };
	int texcrd_idx[3] = {0, };
	int normal_idx[3] = {0, };
	string s[3];

	for (int i=0; i<3; ++i)
		s[i] = token.nextToken();

	CStringTokenizer t[3];
	for (int i=0; i<3; ++i)
		t[i].Init(s[i], split_sign1);

	for (int i=0; i<3; ++i)
	{
		if (s[i].find(split_sign2, 0) == string::npos)
		{
			vertex_idx[i] = atoi(t[i].nextToken().c_str()) -1;
			texcrd_idx[i] = atoi(t[i].nextToken().c_str()) -1;
			normal_idx[i] = atoi(t[i].nextToken().c_str()) -1;
		}
		else // If the string seperated by "//".
		{
			vertex_idx[i] = atoi(t[i].nextToken().c_str()) -1;
			normal_idx[i] = atoi(t[i].nextToken().c_str()) -1;
		}
	}

	return Face(vertex_idx, texcrd_idx, normal_idx);
}