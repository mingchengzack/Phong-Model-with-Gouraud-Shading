#ifndef LINE_H
#define LINE_H

#include "point.h"
#include "glm/glm.hpp"
#include <vector>

class line
{
public:
	point* start;
	point* end;

	line(point* s, point* e); //constructor
	int findInterceptXY(int yvalue); //find the interception given a scanline 
	int findInterceptXZ(int zvalue); //find the interception given a scanline
	int findInterceptYZ(int zvalue); //find the interception given a scanline
	void GedgeXY(); //draw the line on xy plane using linear interpolation
	void GedgeXZ(); //draw the line on xz plane using linear interpolation
	void GedgeYZ(); //draw the line on yz plane using linear interpolation
	glm::vec3 rgb_interceptXY(int x, int y);  //find the interception's rgb value using linear interpolation for xy plane
	glm::vec3 rgb_interceptXZ(int x, int z);  //find the interception's rgb value using linear interpolation for xz plane
	glm::vec3 rgb_interceptYZ(int y, int z);  //find the interception's rgb value using linear interpolation for yz plane
};

#endif

