#include "slider.h"
#include "utility.h"
#include <GL/glut.h>
#include <iostream>
#include <string>

using namespace std;

slider::slider(float x, float y, float max, float min, float length, string name): x(x), y(y), max(max), min(min), length(length)
{
	position = x + length / 40.0f;
	str = name;
}

void slider::drawSlider()
{
	//create slider on the bottom left of the screeen
	glViewport(WinW / 2, 0, WinW / 2, WinH / 2);
	glColor3f(1, 1, 1);
	//draw the label
	drawstr(x - 0.15f, y - 0.02f, str.c_str(), str.length());

	//draw the slider range
	glBegin(GL_LINES);
	glLineWidth(4.5);
	glVertex2f(x, y);
	glVertex2f(x + length / 20.0f, y);
	glEnd();
	//draw the cursor of the slider
	glViewport(WinW / 2, 0, WinW / 2, WinH / 2);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex2f(position - 0.02f, y + 0.02f);
	glVertex2f(position + 0.02f, y + 0.02f);
	glVertex2f(position + 0.02f, y - 0.02f);
	glVertex2f(position - 0.02f, y - 0.02f);
	glEnd();

	string value = to_string(updatedValue()).substr(0, 4);
	drawstr(x + length / 20.0f + 0.05f, y - 0.02f, value.c_str(), value.length());
}

void slider::MoveSlider(int xp)
{
	int xposition;
	if (xp < rd((x + 1.0f) * WinW / 2 / 2.0f) + WinW / 2)
		xposition = rd((x + 1.0f) * WinW / 2 / 2.0f) + WinW / 2;
	else if (xp > rd((x + length / 20.0f + 1.0f) * WinW / 2 / 2.0f) + WinW / 2)
		xposition = rd((x + length / 20.0f + 1.0f) * WinW / 2 / 2.0f) + WinW / 2;
	else
		xposition = xp;
	position = ((xposition - WinW / 2)  * 1.0f) / (WinW / 2 * 1.0f) * 2 - 1.0f;

}

float slider::updatedValue()
{
	float value = (round(((max - min) / (length / 20.0f) * (position - x) + min) * 100) / 100.0f);
	//round to 0 if close to 0
	if (abs(value) < 0.02)
		value = abs(value);
	return value;
}

int slider::getXmax()
{
	int xmax = rd((position + 0.02f + 1.0f) * WinW / 2 / 2.0f) + WinW / 2;
	return xmax;
}

int slider::getYmax()
{
	int ymax = rd((y + 0.02f + 1.0f) * WinH / 2 / 2.0f);
	return ymax;
}

int slider::getXmin()
{
	int xmin = rd((position - 0.02f + 1.0f) * WinW / 2 / 2.0f) + WinW / 2;
	return xmin;
}

int slider::getYmin()
{
	int ymin = rd((y - 0.02f + 1.0f) * WinH / 2 / 2.0f);
	return ymin;
}
