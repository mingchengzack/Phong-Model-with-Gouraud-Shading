#include "object.h"
#include "glm/glm.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

//default constructor
object::object()
{
}

//destructor
object::~object()
{
	//only need to delete point* since line store point*
	for (unsigned int i = vertices.size() - 1; i >= 0; i--)
		delete vertices[i];
}

//compute normal for each vertice
void object::computeNormal()
{
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		//compute the normal to each vertex and store them
		vertices[i]->computeNormal();
	}
}

//compute phong for each vertice
void object::computePhong(glm::vec3 viewpoint, glm::vec3 light, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks,
						  float n, float Ia, float IL, float K)
{
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		//compute the rgb value using phong model to each vertex and store them
		vertices[i]->computePhong(viewpoint,light, ka, kd, ks, n, Ia, IL, K);
	}
	//normalzie rgb
	normalizeRGB();
}

//normalize rgb value
void object::normalizeRGB()
{
	//find the rgb max value in all vertices
	float rgbmax = max(vertices[0]->rgb[0], vertices[0]->rgb[1]);
	rgbmax = max(rgbmax, vertices[0]->rgb[2]);
	float rgb;
	for (unsigned int i = 1; i < vertices.size(); i++)
	{
		rgb = max(vertices[i]->rgb[0], vertices[i]->rgb[1]);
		rgb = max(rgb, vertices[i]->rgb[2]);
		if (rgb > rgbmax)
			rgbmax = rgb;
	}
	//normalize rgb values using rgb max
	for (unsigned int i = 0; i < vertices.size(); i++)
		vertices[i]->normalizeRGB(rgbmax);
}

//change to pixel coordinates
void object::toPixel()
{
	for (unsigned int i = 0; i < vertices.size(); i++)
		vertices[i]->toPixel();
}

//change to world coordinates
void object::toWorld()
{
	for (unsigned int i = 0; i < vertices.size(); i++)
		vertices[i]->toWorld();
}

//overloaded operator for input the object from file
istream& operator>>(istream& is, object& object)
{
	int numOfPoints, numOfFaces;
	float x, y, z;
	int e1, e2, e3;
	is >> numOfPoints;

	//read points and store them
	for (int i = 0; i < numOfPoints; i++)
	{
		is >> x >> y >> z;
		point* p = new point(x, y, z);
		//add it to the vector vertice
		object.vertices.push_back(p);
	}

	is >> numOfFaces;
	//read faces (triangles) and store them
	for (int i = 0; i < numOfFaces; i++)
	{
		is >> e1 >> e2 >> e3;
		triangle* tri = new triangle(object.vertices[e1], object.vertices[e2], object.vertices[e3]);
		tri->addNormal();
		object.faces.push_back(tri);
	}

	return is;
}