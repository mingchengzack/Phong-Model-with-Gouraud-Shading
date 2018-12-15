#ifndef SLIDER_H
#define SLIDER_H

#include <string>

using namespace std;
class slider
{
	float x;
	float y;
	float max;
	float min;
	float length;
	float position;
	string str;
public:
	slider(float x, float y, float max, float min, float length, string name);
	void drawSlider();
	void MoveSlider(int xp);
	float updatedValue();
	int getXmax();
	int getYmax();
	int getXmin();
	int getYmin();
};

#endif

