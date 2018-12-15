#include <cmath>
#include <algorithm>
#include "point.h"
#include "glm/glm.hpp"
#include <iostream>

using namespace std;

//constructor
point::point(float x, float y, float z): x(x), y(y), z(z) {}

//add the normal of a triangle that includes this point
void point::addNormal(glm::vec3 normal)
{
	normals.push_back(normal);
}

//convert to vec3 using glm
glm::vec3 point::toVector3()
{
	return glm::vec3(x, y, z);
}

//compute the normal by averaging 
void point::computeNormal()
{
	//initializes to 0
	glm::vec3 n = glm::vec3(0);
	//add all normals
	for (unsigned int i = 0; i < normals.size(); i++)
		n += normals[i];
	n /= (1.0f * normals.size());
	//normalize the normal
	normal = normalize(n);
}

//phong model computation
void point::computePhong(glm::vec3 viewpoint, glm::vec3 light, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks,
	float n, float Ia, float IL, float K)
{
	glm::vec3 l = normalize(light - toVector3()); 	//light vector
	glm::vec3 r = l * -1.0f + 2.0f * (dot(normal, l)) * normal; //reflection vector
	glm::vec3 v = normalize(viewpoint - toVector3()); //viewing vector
	float cosaphlfa = dot(normal, l);
	float cosbeta =  dot(r, v);
	
	//we dont want negative value for cosa and cosb
	cosaphlfa = max(0.0f, cosaphlfa);
	cosbeta = max(0.0f, cosbeta);
	//compute the intensity using phong model
	rgb = ka * Ia + ((kd * cosaphlfa + ks * pow(cosbeta, n)) * (IL / (length(viewpoint - toVector3()) + K)));
}

//normalize rgb value using rgb max
void point::normalizeRGB(float rgbmax)
{
	rgb = glm::vec3(rgb[0] / rgbmax, rgb[1] / rgbmax, rgb[2] / rgbmax);
}

//convert to pixel coordinates
void point::toPixel()
{
	x = x * 300.0f;
	y = y * 300.0f;
	z = z * 300.0f;
}

//convert to world coordinates
void point::toWorld()
{
	x = x / 300.0f;
	y = y / 300.0f;
	z = z / 300.0f;
}


