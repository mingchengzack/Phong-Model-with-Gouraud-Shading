#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "utility.h"
#include <GL/glut.h>

using namespace std;

//rounding function
int rd(const float a) { return int(a + 0.5); }

//turn pixel(x,y) to (r, g, b) value
void MakePix(int x, int y, float r, float g, float b)
{
	//if it is inside the window
	if (x >= 0 && x < WinW && y >= 0 && y < WinH)
	{
		PixelBuffer[WinW * 3 * y + x * 3] = r;
		PixelBuffer[WinW * 3 * y + x * 3 + 1] = g;
		PixelBuffer[WinW * 3 * y + x * 3 + 2] = b;
	}
}

//draw the string at x, y
void drawstr(float x, float y, const char* format, size_t length)
{
	glRasterPos3f(x, y, 0);
	for (unsigned int i = 0; i < length; i++)
		glutBitmapCharacter(font_style, format[i]);
	glEnd();
}

//find the rgb max inside a mega pixel
int findBlockMax(float block[9][3])
{
	float rgbmax = max(max(block[0][0], block[0][1]), block[0][2]);
	float rgb;
	int index = 0;
	for (int i = 1; i < 9; i++)
	{
		rgb = max(max(block[i][0], block[i][1]), block[i][2]);
		if (rgb > rgbmax)
		{
			rgbmax = rgb;
			index = i;
		}
	}
	return index;
}
