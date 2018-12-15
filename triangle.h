#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "point.h"
#include "line.h"
#include <vector>

class triangle
{
public:
	//the points that form the triangle
	point* p1;
	point* p2;
	point* p3;
	glm::vec3 n;  //the normal to the faces
	//edges of the triangles
	vector<line*> edges; //the edges for the triangle

	triangle(point* P1, point* P2, point* P3);
	
	float findZmin(); //find the max z
	float findYmax(); //find the max y
	float findXmin(); //find the max x
	
	void GshadingXY(); //Gouraud shading the triangle and project to xy plane
	void GshadingXZ(); //Gouraud shading the triangle and project to xz plane
	void GshadingYZ(); //Gouraud shading the triangle and project to yz plane
	
	void GedgeXY();  //draw out the triangle using linear interpolation on xy plane
	void GedgeXZ(); //draw out the triangle using linear interpolation on xz plane
	void GedgeYZ(); //draw out the triangle using linear interpolation on yz plane

	void addNormal(); //add normal to the triangle to each vertex
};

#endif

