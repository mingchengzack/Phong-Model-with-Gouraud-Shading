#ifndef OBJECT_H
#define OBJECT_H
#include <cstdio>
#include <fstream>

#include "glm/glm.hpp"
#include "point.h"
#include "triangle.h"

using namespace std;

class object
{
public:
	vector<point*> vertices;
	vector<triangle*> faces;

	object(); //default constructor
	~object();  //default destructor
	void computeNormal(); //compute the normal to each vertex in the object
	void computePhong(glm::vec3 viewpoint, glm::vec3 light, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks,
					  float n, float Ia, float IL, float K); //compute the light intensity for each vertex in the object
	void normalizeRGB(); //normalnize all the rgb values for the points
	void toPixel(); //change all vertices to pixel coordinates
	void toWorld(); //change to world coordinates
	friend istream& operator>>(istream& is, object& object); //operator for reading input file
};

#endif
