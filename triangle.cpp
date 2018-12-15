#include "triangle.h"
#include "glm/glm.hpp"
#include "utility.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

//constructor
triangle::triangle(point* P1, point* P2, point* P3)
{
	p1 = P1;
	p2 = P2;
	p3 = P3;
	line* edge1= new line(p1, p2); //point 1 to 2
	line* edge2 = new line(p1, p3);  //point 1 to 3
	line* edge3 = new line(p2, p3); //point 2 to 3
	edges.push_back(edge1);
	edges.push_back(edge2);
	edges.push_back(edge3);
	//calculate the normal to the surface
	n = cross(p2->toVector3() - p1->toVector3(), p3->toVector3() - p1->toVector3());
}

//find the min z value for painter
float triangle::findZmin()
{
	float zmin = min(p1->z, p2->z);
	zmin = min(zmin, p3->z);
	return zmin;
}

//find the min y value
float triangle::findYmax()
{
	float ymax = max(p1->y, p2->y);
	ymax = max(ymax, p3->y);
	return ymax;
}

//find the min x value
float triangle::findXmin()
{
	float xmin = min(p1->x, p2->x);
	xmin = min(xmin, p3->x);
	return xmin;
}

//Gouraud shading the triangle and project to xy plane
void triangle::GshadingXY()
{
	int ymax, ymin;
	vector<int> interceptions;
	vector<glm::vec3> rgb;
	ymax = rd(max(max(p1->y, p2->y), p3->y));
	ymin = rd(min(min(p1->y, p2->y), p3->y));
	//scanline from ymin to ymax
	for (int i = ymin; i <= ymax; i++)
	{
		int x;
		//find interceptions for each edge 
		for (unsigned int j = 0; j < edges.size(); j++)
		{
			x = edges[j]->findInterceptXY(i);
			if (x != -1)
			{
				bool found = false;
				//dont add repetition 
				for (unsigned int i = 0; i < interceptions.size(); i++)
				{
					if (interceptions[i] == x)
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					unsigned int k;
					for (k = 0; k < interceptions.size(); k++)
					{
						if (x < interceptions[k])
							break;
					}
					//add interceptions and rgb value
					interceptions.insert(interceptions.begin() + k, x);
					rgb.insert(rgb.begin() + k, edges[j]->rgb_interceptXY(x, i));
				}

			}
		}
		//intercept one point
		if (interceptions.size() == 1)
			MakePix(interceptions[0], i + WinH / 2, rgb[0][0], rgb[0][1], rgb[0][2]);
		//intercept two points
		else if (interceptions.size() == 2)
		{
			glm::vec3 rgb_between = glm::vec3(0);
			int dx;
			dx = interceptions[1] - interceptions[0];
			for (int k = interceptions[0]; k <= interceptions[1]; k++)
			{
				//linear interpolation
				rgb_between = (interceptions[1] - k) * 1.0f / (dx * 1.0f) * rgb[0] + (k - interceptions[0]) * 1.0f / (dx * 1.0f) * rgb[1];
				MakePix(k , i + WinH / 2, rgb_between[0], rgb_between[1], rgb_between[2]);
			}
		}
		//intercept three points
		else
		{
			glm::vec3 rgb_between = glm::vec3(0);
			int dx;
			dx = interceptions[2] - interceptions[0];
			for (int k = interceptions[0]; k <= interceptions[2]; k++)
			{
				//linear interpolation
				rgb_between = (interceptions[2] - k) * 1.0f / (dx * 1.0f) * rgb[0] + (k - interceptions[0]) * 1.0f / (dx * 1.0f) * rgb[2];
				MakePix(k, i + WinH / 2, rgb_between[0], rgb_between[1], rgb_between[2]);
			}
		}
		interceptions.clear();
		rgb.clear();	
	}
}

//Gouraud shading the triangle and project to xz plane
void triangle::GshadingXZ()
{
	int zmax, zmin;
	vector<int> interceptions;
	vector<glm::vec3> rgb;
	zmax = rd(max(max(p1->z, p2->z), p3->z));
	zmin = rd(min(min(p1->z, p2->z), p3->z));
	//scanline from zmin to zmax
	for (int i = zmin; i <= zmax; i++)
	{
		int x;
		//find interceptions for each edge 
		for (unsigned int j = 0; j < edges.size(); j++)
		{
			x = edges[j]->findInterceptXZ(i);
			if (x != -1)
			{
				bool found = false;
				//dont add repetition 
				for (unsigned int i = 0; i < interceptions.size(); i++)
				{
					if (interceptions[i] == x)
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					unsigned int k;
					for (k = 0; k < interceptions.size(); k++)
					{
						if (x < interceptions[k])
							break;
					}
					//add interceptions and rgb value
					interceptions.insert(interceptions.begin() + k, x);
					rgb.insert(rgb.begin() + k, edges[j]->rgb_interceptXZ(x, i));
				}

			}
		}
		if (interceptions.size() == 1)
			MakePix(interceptions[0] + WinW / 2, i + WinH / 2, rgb[0][0], rgb[0][1], rgb[0][2]);
		else if (interceptions.size() == 2)
		{
			glm::vec3 rgb_between = glm::vec3(0);
			int dx;
			dx = interceptions[1] - interceptions[0];
			for (int k = interceptions[0]; k <= interceptions[1]; k++)
			{
				//linear interpolation
				rgb_between = (interceptions[1] - k) * 1.0f / (dx * 1.0f) * rgb[0] + (k - interceptions[0]) * 1.0f / (dx * 1.0f) * rgb[1];
				MakePix(k + WinW / 2, i + WinH / 2, rgb_between[0], rgb_between[1], rgb_between[2]);
			}
		}
		else
		{
			glm::vec3 rgb_between = glm::vec3(0);
			int dx;
			dx = interceptions[2] - interceptions[0];
			for (int k = interceptions[0]; k <= interceptions[2]; k++)
			{
				//linear interpolation
				rgb_between = (interceptions[2] - k) * 1.0f / (dx * 1.0f) * rgb[0] + (k - interceptions[0]) * 1.0f / (dx * 1.0f) * rgb[2];
				MakePix(k + WinW / 2, i + WinH / 2, rgb_between[0], rgb_between[1], rgb_between[2]);
			}
		}
		interceptions.clear();
		rgb.clear();
	}
}

//Gouraud shading the triangle and project to yz plane
void triangle::GshadingYZ()
{
	int zmax, zmin;
	vector<int> interceptions;
	vector<glm::vec3> rgb;
	zmax = rd(max(max(p1->z, p2->z), p3->z));
	zmin = rd(min(min(p1->z, p2->z), p3->z));
	//scanline from zmin to zmax
	for (int i = zmin; i <= zmax; i++)
	{
		int y;
		//find interceptions for each edge 
		for (unsigned int j = 0; j < edges.size(); j++)
		{
			y = edges[j]->findInterceptYZ(i);
			if (y != -1)
			{
				bool found = false;
				//dont add repetition 
				for (unsigned int i = 0; i < interceptions.size(); i++)
				{
					if (interceptions[i] == y)
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					unsigned int k;
					for (k = 0; k < interceptions.size(); k++)
					{
						if (y < interceptions[k])
							break;
					}
					//add interceptions and rgb value
					interceptions.insert(interceptions.begin() + k, y);
					rgb.insert(rgb.begin() + k, edges[j]->rgb_interceptYZ(y, i));
				}

			}
		}
		if (interceptions.size() == 1)
			MakePix(interceptions[0], i, rgb[0][0], rgb[0][1], rgb[0][2]);
		else if (interceptions.size() == 2)
		{
			glm::vec3 rgb_between = glm::vec3(0);
			int dy;
			dy = interceptions[1] - interceptions[0];
			for (int k = interceptions[0]; k <= interceptions[1]; k++)
			{
				//linear interpolation
				rgb_between = (interceptions[1] - k) * 1.0f / (dy * 1.0f) * rgb[0] + (k - interceptions[0]) * 1.0f / (dy * 1.0f) * rgb[1];
				MakePix(k, i, rgb_between[0], rgb_between[1], rgb_between[2]);
			}
		}
		else 
		{
			glm::vec3 rgb_between = glm::vec3(0);
			int dy;
			dy = interceptions[2] - interceptions[0];
			for (int k = interceptions[0]; k <= interceptions[2]; k++)
			{
				//linear interpolation
				rgb_between = (interceptions[2] - k) * 1.0f / (dy * 1.0f) * rgb[0] + (k - interceptions[0]) * 1.0f / (dy * 1.0f) * rgb[2];
				MakePix(k, i, rgb_between[0], rgb_between[1], rgb_between[2]);
			}
		}
		interceptions.clear();
		rgb.clear();
	}
}

//draw out the triangle using linear interpolation on xy plane
void triangle::GedgeXY()
{
	//draw each edge
	for (unsigned int i = 0; i < edges.size(); i++)
		edges[i]->GedgeXY();
}

//draw out the triangle using linear interpolation on xz plane
void triangle::GedgeXZ()
{
	//draw each edge
	for (unsigned int i = 0; i < edges.size(); i++)
		edges[i]->GedgeXZ();
}

//draw out the triangle using linear interpolation on yz plane
void triangle::GedgeYZ()
{
	//draw each edge
	for (unsigned int i = 0; i < edges.size(); i++)
		edges[i]->GedgeYZ();
}

//add the normal to the triangle to each vertex
void triangle::addNormal()
{
	//find the normal to p1 using p1 to p2 and p1 to p3
	glm::vec3 normal = cross(p2->toVector3() - p1->toVector3(), p3->toVector3() - p1->toVector3());
	//add this normal to each vertex
	p1->addNormal(normal);
	//find the normal to p2 using p2 to p3 and p2 to p1
	normal = cross(p3->toVector3() - p2->toVector3(), p1->toVector3() - p2->toVector3());
	p2->addNormal(normal);
	//find the normal to p3 using p3 to p1 and p3 to p2
	normal = cross(p1->toVector3() - p3->toVector3(), p2->toVector3() - p3->toVector3());
	p3->addNormal(normal);
}

