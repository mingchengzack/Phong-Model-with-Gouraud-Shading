#include "line.h"
#include "utility.h"
#include "glm/glm.hpp"
#include <algorithm>
#include <iostream>

using namespace std;

//constructor
line::line(point* s, point* e) 
{
	start = s;
	end = e;
}

//find the interception on xy plane given a scanline value
int line::findInterceptXY(int yvalue)
{
	//check if it intercepts
	if(yvalue > rd(max(start->y, end->y)) || yvalue < rd(min(start->y, end->y)))
		return -1;
	//DDA to find interception
	float dx = end->x - start->x, dy = end->y - start->y, steps;
	float xIncrement, yIncrement, x = start->x, y = start->y;

	if (fabs(dx) > fabs(dy))
		steps = fabs(dx);
	else
		steps = fabs(dy);
	xIncrement = dx / steps;
	yIncrement = dy / steps;
	while (rd(y) != yvalue)
	{
		x += xIncrement;
		y += yIncrement;
	}
	return rd(x);
}

//find the interception on xz plane given a scanline value
int line::findInterceptXZ(int zvalue)
{
	//check if intercepts
	if (zvalue > rd(max(start->z, end->z)) || zvalue < rd(min(start->z, end->z)))
		return -1;
	//DDA to find intercepts
	float dx = end->x - start->x, dz = end->z - start->z, steps;
	float xIncrement, zIncrement, x = start->x, z = start->z;

	if (fabs(dx) > fabs(dz))
		steps = fabs(dx);
	else
		steps = fabs(dz);
	xIncrement = dx / steps;
	zIncrement = dz / steps;
	while (rd(z) != zvalue)
	{
		x += xIncrement;
		z += zIncrement;
	}
	return rd(x);
}

//find the interception on yz plane given a scanline value
int line::findInterceptYZ(int zvalue)
{
	//check if intercepts
	if (zvalue > rd(max(start->z, end->z)) || zvalue < rd(min(start->z, end->z)))
		return -1;
	//DDA to find intercepts
	float dy = end->y - start->y, dz = end->z - start->z, steps;
	float yIncrement, zIncrement, y = start->y, z = start->z;

	if (fabs(dy) > fabs(dz))
		steps = fabs(dy);
	else
		steps = fabs(dz);
	yIncrement = dy / steps;
	zIncrement = dz / steps;
	while (rd(z) != zvalue)
	{
		y += yIncrement;
		z += zIncrement;
	}
	return rd(y);
}

//draw the line on xy plane using linear interpolation
void line::GedgeXY()
{
	float dx = end->x - start->x, dy = end->y - start->y, steps;
	float xIncrement, yIncrement, x = start->x, y = start->y;
	glm::vec3 intercept_rgb = glm::vec3(0);
	if (fabs(dx) > fabs(dy))
		steps = fabs(dx);
	else
		steps = fabs(dy);
	xIncrement = dx / steps;
	yIncrement = dy / steps;
	MakePix(rd(x), rd(y) + WinH / 2, start->rgb[0], start->rgb[1], start->rgb[2]);
	//DDA progress
	for (int i = 0; i < rd(steps) - 1; i++)
	{
		x += xIncrement;
		y += yIncrement;
		//linear interpolation
		if (fabs(dx) > fabs(dy))
			intercept_rgb = (end->x - x) / dx * start->rgb + (x - start->x) / dx * end->rgb;
		else
			intercept_rgb = (end->y - y) / dy * start->rgb + (y - start->y) / dy * end->rgb;
		MakePix(rd(x), rd(y) + WinH / 2, intercept_rgb[0], intercept_rgb[1], intercept_rgb[2]);
	}	
}

//draw the line on xz plane using linear interpolation
void line::GedgeXZ()
{
	float dx = end->x - start->x, dz = end->z - start->z, steps;
	float xIncrement, zIncrement, x = start->x, z = start->z;
	glm::vec3 intercept_rgb = glm::vec3(0);
	if (fabs(dx) > fabs(dz))
		steps = fabs(dx);
	else
		steps = fabs(dz);
	xIncrement = dx / steps;
	zIncrement = dz / steps;
	MakePix(rd(x) + WinW / 2, rd(z) + WinH / 2, start->rgb[0], start->rgb[1], start->rgb[2]);
	//DDA progress
	for (int i = 0; i < rd(steps) - 1; i++)
	{
		x += xIncrement;
		z += zIncrement;
		//linear interpolation
		if (fabs(dx) > fabs(dz))
			intercept_rgb = (end->x - x) / dx * start->rgb + (x - start->x) / dx * end->rgb;
		else
			intercept_rgb = (end->z - z) / dz * start->rgb + (z - start->z) / dz * end->rgb;
		MakePix(rd(x) + WinW / 2, rd(z) + WinH / 2, intercept_rgb[0], intercept_rgb[1], intercept_rgb[2]);
	}
}

//draw the line on yz plane using linear interpolation
void line::GedgeYZ()
{
	float dy = end->y - start->y, dz = end->z - start->z, steps;
	float yIncrement, zIncrement, y = start->y, z = start->z;
	glm::vec3 intercept_rgb = glm::vec3(0);
	if (fabs(dy) > fabs(dz))
		steps = fabs(dy);
	else
		steps = fabs(dz);
	yIncrement = dy / steps;
	zIncrement = dz / steps;
	//DDA progress
	MakePix(rd(y), rd(z), start->rgb[0], start->rgb[1], start->rgb[2]);
	for (int i = 0; i < rd(steps) - 1; i++)
	{
		y += yIncrement;
		z += zIncrement;
		//linear interpolation
		if (fabs(dy) > fabs(dz))
			intercept_rgb = (end->y - y) / dy * start->rgb + (y- start->y) / dy * end->rgb;
		else
			intercept_rgb = (end->z - z) / dz * start->rgb + (z - start->z) / dz * end->rgb;
		MakePix(rd(y), rd(z), intercept_rgb[0], intercept_rgb[1], intercept_rgb[2]);
	}
}

//calculate the interceptions' rgb based on start point and end point using linear interpolation for xy plane
glm::vec3 line::rgb_interceptXY(int x, int y)
{
	glm::vec3 intercept_rgb = glm::vec3(0);
	float dx = end->x - start->x, dy = end->y - start->y;
	if (fabs(dx) > fabs(dy))
		intercept_rgb = (end->x - x * 1.0f) / dx * start->rgb + (x * 1.0f - start->x) / dx * end->rgb;
	else
		intercept_rgb = (end->y - y * 1.0f) / dy * start->rgb + (y * 1.0f - start->y) / dy * end->rgb;
	return intercept_rgb;
}

//calculate the interceptions' rgb based on start point and end point using linear interpolation for xz plane
glm::vec3 line::rgb_interceptXZ(int x, int z)
{
	glm::vec3 intercept_rgb = glm::vec3(0);
	float dx = end->x - start->x, dz = end->z - start->z;
	if (fabs(dx) > fabs(dz))
		intercept_rgb = (end->x - x * 1.0f) / dx * start->rgb + (x * 1.0f - start->x) / dx * end->rgb;
	else
		intercept_rgb = (end->z - z * 1.0f) / dz * start->rgb + (z * 1.0f - start->z) / dz * end->rgb;
	return intercept_rgb;
}

//calculate the interceptions' rgb based on start point and end point using linear interpolation for yz plane
glm::vec3 line::rgb_interceptYZ(int y, int z)
{
	glm::vec3 intercept_rgb = glm::vec3(0);
	float dy = end->y - start->y, dz = end->z - start->z;
	if (fabs(dy) > fabs(dz))
		intercept_rgb = (end->y - y * 1.0f) / dy * start->rgb + (y * 1.0f - start->y) / dy * end->rgb;
	else
		intercept_rgb = (end->z - z * 1.0f) / dz * start->rgb + (z * 1.0f - start->z) / dz * end->rgb;
	return intercept_rgb;
}
