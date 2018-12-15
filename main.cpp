#include <GL/glut.h>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>

#include "glm/glm.hpp"
#include "object.h"
#include "utility.h"
#include "slider.h"

using namespace std;

GLvoid *font_style = GLUT_BITMAP_HELVETICA_12; //font style
void display(); //display function
float* PixelBuffer; //the pixel buffer for normal shading
float* halfTone;  //the pixel buffer for half toning
int WinW = 900;  //window width
int WinH = 900;  //window height
int option = 1; //half-tone on or off
glm::vec3 viewpoint; //view position
glm::vec3 light;  //light position
//phong model constants
glm::vec3 ka;  
glm::vec3 kd;
glm::vec3 ks;
float n;
int select = -1;
float Ia, IL, K;

vector<slider> sliders; //sliders
vector<object*> objects;  //3D objects
vector<triangle*> faces;  //all the triangles
void sortXY(); //sort the triangles using z depths
void sortXZ();  //sort the triangles using y depths
void sortYZ();  //sort the triangles using x depths
void KeyBoardFunc(unsigned char key, int x, int y);  //the keyboard function
void MouseFunc(int button, int state, int x, int y);  //the mouse function
void MotionFunc(int x, int y); //mouse motion function
float* halfToning(); //compute the half tone buffer
void initSliders();  //initialize sliders
void drawPlaneLabel(); //draw the plane labels and grid
void drawSliders();  //draw the sliders
void projectObjects();  //shading the object and compute half toning
void display(); //display function

//sort all the triangles using z values (min value first)
void sortXY()
{
	float zmin;
	unsigned int index;
	triangle* tri;
	//insertion sort
	for (unsigned int i = 0; i < faces.size() - 1; i++)
	{
		zmin = faces[i]->findZmin();
		index = i;
		for (unsigned int j = i + 1; j < faces.size(); j++)
		{
			if (faces[j]->findZmin() <= zmin)
			{
				zmin = faces[j]->findZmin();
				index = j;
			}
		}
		tri = faces[index];
		faces.erase(faces.begin() + index);
		faces.insert(faces.begin() + i, tri);
	}
}

//sort all the triangles using y values (min value first)
void sortXZ()
{
	float ymax;
	unsigned int index;
	triangle* tri;
	//insertion sort
	for (unsigned int i = 0; i < faces.size() - 1; i++)
	{
		ymax = faces[i]->findYmax();
		index = i;
		for (unsigned int j = i + 1; j < faces.size(); j++)
		{
			if (faces[j]->findYmax() >= ymax)
			{
				ymax = faces[j]->findYmax();
				index = j;
			}
		}
		tri = faces[index];
		faces.erase(faces.begin() + index);
		faces.insert(faces.begin() + i, tri);
	}
}

//sort all the triangles using x values (min value first)
void sortYZ()
{
	float xmin;
	unsigned int index;
	triangle* tri;
	//insertion sort
	for (unsigned int i = 0; i < faces.size() - 1; i++)
	{
		xmin = faces[i]->findXmin();
		index = i;
		for (unsigned int j = i + 1; j < faces.size(); j++)
		{
			if (faces[j]->findXmin() <= xmin)
			{
				xmin = faces[j]->findXmin();
				index = j;
			}
		}
		tri = faces[index];
		faces.erase(faces.begin() + index);
		faces.insert(faces.begin() + i, tri);
	}
}

//the keyboard function for switching normal and half tone
void KeyBoardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
		//half tone is off
		case '1':
		{
			option = 1;
			glutPostRedisplay();
			break;
		}
		//half tone is on
		case '2':
		{
			option = 2;
			glutPostRedisplay();
			break;
		}

		case 27:
		{
			exit(0);
		}

		case 'a':
		{
			break;
		}
	}
}

//mouse function for sliders
void MouseFunc(int button, int state, int x, int y)
{
	int yp = WinH - y, xp = x;
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
		{
			//press down the mouse
			if (state == GLUT_DOWN)
			{
				//inside the cursor box
				for (unsigned int i = 0; i < sliders.size(); i++)
				{
					if (yp >= sliders[i].getYmin() && yp <= sliders[i].getYmax()
						&& xp <= sliders[i].getXmax() && xp >= sliders[i].getXmin())
						select = i;
				}
			}
			//release
			else if (state == GLUT_UP)
			{
				if (select != -1)
				{
					select = -1;
					//project the new phong model
					projectObjects();
					glutPostRedisplay();
				}
			}
			
			break;
		}
	}
}

//motion function for sliders
void MotionFunc(int x, int y)
{
	int xp = x;
	if (select != -1)
	{
		//move sliders
		sliders[select].MoveSlider(xp);
		//update parameters
		ka = glm::vec3(sliders[0].updatedValue(), sliders[1].updatedValue(), sliders[2].updatedValue());
		kd = glm::vec3(sliders[3].updatedValue(), sliders[4].updatedValue(), sliders[5].updatedValue());
		ks = glm::vec3(sliders[6].updatedValue(), sliders[7].updatedValue(), sliders[8].updatedValue());
		Ia = sliders[9].updatedValue();
		IL = sliders[10].updatedValue();
		n = sliders[11].updatedValue();
		light = glm::vec3(sliders[12].updatedValue(), sliders[13].updatedValue(), sliders[14].updatedValue());
		viewpoint = glm::vec3(sliders[15].updatedValue(), sliders[16].updatedValue(), sliders[17].updatedValue());
		K = sliders[18].updatedValue();
		glutPostRedisplay();
	}
}

//compute the half toning using mega pixels
float* halfToning()
{
	float* halfTone = new float[WinW * WinH * 3];
	float block[9][3];
	int row, col;
	//divide WinW x WinH into WinW / 3 x WinH / 3 mega pixels
	for (int i = 0; i < WinW * WinH / 9; i++)
	{
		int x, y, index, num_activate;
		//find the row col in pixel buffers
		row = i / (WinH / 3) + (i / (WinH / 3)) * 2;
		col = (i % (WinW / 3)) * 3;
		//initializes block
		for (int j = 0; j < 9; j++)
		{
			x = col + j % 3;
			y = row + j / 3;
			block[j][0] = PixelBuffer[WinW * 3 * y + x * 3];
			block[j][1] = PixelBuffer[WinW * 3 * y + x * 3 + 1];
			block[j][2] = PixelBuffer[WinW * 3 * y + x * 3 + 2];
		}
		index = findBlockMax(block);
		//rounding down
		num_activate = (int)(max(max(block[index][0], block[index][1]), block[index][2]) * 9);
		//not turnning on any pixel in the mega pixels
		if (num_activate == 0)
			continue;
		//randomize the pixels to be turned on
		vector<int> choices;
		//random seed
		srand((unsigned int)(time(NULL) + i));
		int pick;
		//randomize the pixels to be turned on
		for (int k = 0; k < num_activate; k++)
		{
			bool repeat = true;
			//make sure no repetition
			while (repeat)
			{
				pick = rand() % 9;
				repeat = false;
				for (unsigned int j = 0; j < choices.size(); j++)
				{
					if (choices[j] == pick)
					{
						repeat = true;
						break;
					}
				}
			}
			choices.push_back(pick);
		}

		//activate on the halftone pixel buffer
		for (unsigned int j = 0; j < choices.size(); j++)
		{
			x = col + choices[j] % 3;
			y = row + choices[j] / 3;
			halfTone[WinW * 3 * y + x * 3] = block[index][0];
			halfTone[WinW * 3 * y + x * 3 + 1] = block[index][1];
			halfTone[WinW * 3 * y + x * 3 + 2] = block[index][2];
		}

	}
	return halfTone;
}

//initialize sliders
void initSliders()
{
	//initializes all the sliders for all the inputs for phong model
	slider ka_r(-0.75f, 0.9f, 1.0f, 0.0f, 10.0f, "ka_r");
	slider ka_g(-0.75f, 0.8f, 1.0f, 0.0f, 10.0f, "ka_g");
	slider ka_b(-0.75f, 0.7f, 1.0f, 0.0f, 10.0f, "ka_b");
	slider kd_r(-0.75f, 0.6f, 1.0f, 0.0f, 10.0f, "kd_r");
	slider kd_g(-0.75f, 0.5f, 1.0f, 0.0f, 10.0f, "kd_g");
	slider kd_b(-0.75f, 0.4f, 1.0f, 0.0f, 10.0f, "kd_b");
	slider ks_r(-0.75f, 0.3f, 1.0f, 0.0f, 10.0f, "ks_r");
	slider ks_g(-0.75f, 0.2f, 1.0f, 0.0f, 10.0f, "ks_g");
	slider ks_b(-0.75f, 0.1f, 1.0f, 0.0f, 10.0f, "ks_b");
	slider Ia(-0.75f, 0.0f, 1.0f, 0.0f, 10.0f, "Ia");
	slider IL(-0.75f, -0.1f, 1.0f, 0.0f, 10.0f, "IL");
	slider n(-0.75f, -0.2f, 10.0f, 0.005f, 10.0f, "n");

	//sliders for light position
	slider light_x(0.25f, 0.9f, 5.0f, -5.0f, 10.0f, "l_x");
	slider light_y(0.25f, 0.8f, 5.0f, -5.0f, 10.0f, "l_y");
	slider light_z(0.25f, 0.7f, 5.0f, -5.0f, 10.0f, "l_z");
	//sliders for view position
	slider view_x(0.25f, 0.6f, 5.0f, -5.0f, 10.0f, "v_x");
	slider view_y(0.25f, 0.5f, 5.0f, -5.0f, 10.0f, "v_y");
	slider view_z(0.25f, 0.4f, 5.0f, -5.0f, 10.0f, "v_z");
	slider K(-0.75f, -0.3f, 2.0f, 0.0f, 10.0f, "K");

	//add all the sliders
	sliders.push_back(ka_r);
	sliders.push_back(ka_g);
	sliders.push_back(ka_b);
	sliders.push_back(kd_r);
	sliders.push_back(kd_g);
	sliders.push_back(kd_b);
	sliders.push_back(ks_r);
	sliders.push_back(ks_g);
	sliders.push_back(ks_b);
	sliders.push_back(Ia);
	sliders.push_back(IL);
	sliders.push_back(n);
	sliders.push_back(light_x);
	sliders.push_back(light_y);
	sliders.push_back(light_z);
	sliders.push_back(view_x);
	sliders.push_back(view_y);
	sliders.push_back(view_z);
	sliders.push_back(K);
}

//main program: read files, set default parameters for phong and project objects 
int main(int argc, char *argv[])
{
	int numOfObjects;
	//default value for phong models
	viewpoint = glm::vec3(0, 0, 0);
	light = glm::vec3(0, 0, 0);
	ka = glm::vec3(0.5f, 0.5f, 0.5f);
	kd = glm::vec3(0.5f, 0.5f, 0.5f);
	ks = glm::vec3(0.5f, 0.5f, 0.5f);
	n = 5.0;
	Ia = 0.5f, IL = 0.5f, K = 1.0f;
	//allocate buffer
	PixelBuffer = new float[WinW * WinH * 3];
	//initialize to black
	for (int i = 0; i < WinW * WinH * 3; i++)
		PixelBuffer[i] = 0;
	//read in file 
	ifstream inf;
	inf.open("test.txt");

	inf >> numOfObjects;
	for (int i = 0; i < numOfObjects; i++)
	{
		object *obj = new object();

		inf >> *obj;
		obj->computeNormal();
		for (unsigned int j = 0; j < obj->faces.size(); j++)
			faces.push_back(obj->faces[j]);
		objects.push_back(obj);
	}

	inf.close();

	//project the objects when program runs for the first time
	projectObjects();
	//initialize sliders
	initSliders();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	//set window size to 500*500
	glutInitWindowSize(WinW, WinH);
	//set window position
	glutInitWindowPosition(100, 100);
	//create and set main window title
	int MainWindow = glutCreateWindow("Project 3!");

	glClearColor(1, 1, 1, 0); //clears the buffer of OpenGL

	glutKeyboardFunc(KeyBoardFunc); //keyboard function

	glutMouseFunc(MouseFunc); //mouse function
	
	glutMotionFunc(MotionFunc); //motion function

	glutDisplayFunc(display); //display function

	glutMainLoop();//main display loop, will display until terminate

	return 0;
}

//draw the plane labels and grid
void drawPlaneLabel()
{
	//draw plane labels
	glViewport(0, WinH / 2, WinW / 2, WinH / 2);
	glColor3f(1, 0, 0);
	string str = "XY Plane";
	drawstr(-0.2f, 0.9f, str.c_str(), str.length());

	glViewport(WinW / 2, WinH / 2, WinW / 2, WinH / 2);
	glColor3f(0, 1, 0);
	str = "XZ Plane";
	drawstr(-0.2f, 0.9f, str.c_str(), str.length());

	glViewport(0, 0, WinW / 2, WinH / 2);
	glColor3f(0, 0, 1);
	str = "YZ Plane";
	drawstr(-0.2f, 0.9f, str.c_str(), str.length());
	//DRAW GRID ON SCREEN
	glViewport(0, 0, WinW, WinH);

	//draw grid
	glBegin(GL_LINES);
	glColor3f(0.5, 0.5, 0.5);
	glVertex2f(0, -1);
	glVertex2f(0, 1);
	glVertex2f(-1, 0);
	glVertex2f(1, 0);
	glEnd();

	//draw user information for half tone
	glColor3f(1, 1, 1);
	glViewport(WinW / 2, 0, WinW / 2, WinH / 2);
	str = "Press 1 for Normal Shading";
	drawstr(0.25f, -0.1f, str.c_str(), str.length());
	str = "Press 2 for Half Toning";
	drawstr(0.25f, -0.2f, str.c_str(), str.length());
	str = "Press esc to exit";
	drawstr(0.25f, -0.3f, str.c_str(), str.length());
}

//draw the sliders
void drawSliders()
{
	for (unsigned int i = 0; i < sliders.size(); i++)
		sliders[i].drawSlider();
}

//project the objects
void projectObjects()
{
	//compute the phong models using world coordinates
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->computePhong(viewpoint, light, ka, kd, ks, n, Ia, IL, K);
		//transfer to pixel coordinates for shading
		objects[i]->toPixel();
	}

	//sort the faces according to z depths
	sortXY();
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		if (dot(faces[i]->n, glm::vec3(0, 0, 1)) > 0)
		{
			//draw the edge
			faces[i]->GedgeXY();
			//shading the interior
			faces[i]->GshadingXY();
		}
	}

	//sort the faces according to y depths
	sortXZ();
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		if (dot(faces[i]->n, glm::vec3(0, -1, 0)) > 0)
		{
			//draw the edge
			faces[i]->GedgeXZ();
			//shading the interior
			faces[i]->GshadingXZ();
		}
	}

	//sort the faces according to x depths
	sortYZ();
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		if (dot(faces[i]->n, glm::vec3(1, 0, 0)) > 0)
		{
			//draw the edge
			faces[i]->GedgeYZ();
			//shading the interior
			faces[i]->GshadingYZ();
		}
	}

	//convert back to world coordinates for next phong calculation
	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->toWorld();

	//half toning
	halfTone = halfToning();
}

//display function
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//draws pixel on screen
	glViewport(0, 0, WinW, WinH);
	glRasterPos3f(-1.0f, -1.0f, 0.0f);

	if(option == 1)
		glDrawPixels(WinW, WinH, GL_RGB, GL_FLOAT, PixelBuffer);
	else
	{
		glDrawPixels(WinW, WinH, GL_RGB, GL_FLOAT, halfTone);
	}
	
	//draw the plane labels
	drawPlaneLabel();
	//draw all the sliders
	drawSliders();
	
	//window refresh
	glFlush();
}

