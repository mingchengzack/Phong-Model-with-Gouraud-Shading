#ifndef POINT_H
#define POINT_H

#include <vector>
#include "glm/glm.hpp"

using namespace std;

class point
{
public:
	float x;
	float y;
	float z;
	vector <glm::vec3> normals; //storing all the surfaces normals
	glm::vec3 normal;  //the actual normal
	glm::vec3 rgb;    //the rgb intensity value calculated using phong
	point(float x, float y, float z); //constructor
	void addNormal(glm::vec3 normal); //add the normal of a triangle that includes this point
	glm::vec3 toVector3(); //return the glm vec3 of the point
	void computeNormal(); //compute the normal of the vertex given all the normals for other faces
	void computePhong(glm::vec3 viewpoint, glm::vec3 light, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks,
					  float n, float Ia, float IL, float K); //compute the light intensity of the vertex given all constants
	void normalizeRGB(float rgbmax); //normalize the rgb value to 0 to 1
	void toPixel(); //change to pixel buffer
	void toWorld(); //change to world coordinates
};

#endif

