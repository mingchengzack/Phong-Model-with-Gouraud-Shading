# Phong lighting mode
Gouraud shading for rasterizing shaded triangles,half-toning for simulating different brightness on a binary display device, and the painter’s algorithm for solving the hidden surface 
To compile my project, just type make, and it will output a executable called project3.

To run my project, just type in ./project3. My program has a input file called test.txt.

format of input file: (test.txt)
3              number of objects
4 	       number of vertices for this object
0.53 0.3 0.35    vertice information
0.65 0.32 0.63
0.77 0.14 0.21
0.53 0.40 0.07
4             number of faces
0 1 2         how to connect the faces using vertex
0 2 3
0 3 1
1 3 2

Since I didn't include the normals information for the vertice, I manually calculate
them using the orientation of for the faces provided in input file. For computing the
normals, I used all the normals (outward) for the triangle this vertex connects to 
calculate. I have the correct orientation for the triangle, so if the orientation is 
changed, my normals will be off!!!

My user menu, which is on the bottom right, enables the user to set the parameters for
the phong model using sliders. 

!!How to use sliders:
Just click the cube box on the parameter and drag to the value you want, after you 
release the mouse, it will show the new phong model using the parameters you set.

To turn the half toning off or on, just press "1" key for normal shading, and press
"2" key for half toning shading.

Lines for implementing algorithms:
For computing normals: 
	triangle.cpp: line 300 - 312
	object.cpp: line 22 - 29
	point.cpp: line 13 - 16, line 25 - 35
For computing phong model:
	object.cpp: line 32 - 61
	point.cpp: 38 - 58
For Gouraud Shading(draw out the shape and filling up):
	draw out the shape:
		triangle.cpp: line 276 - 297
		line.cpp: line 89 - 167
	filling up:
		triangle.cpp: line 51 - 273
		line.cpp: line 17 - 86, line 170 - 203
For Painter's Algorithm:
	Sorting(insertion sort):
		main.cpp: line 52 - 124
	Orthographic projection:
		main.cpp: line 434 - 489

I implemented a class called slider to do the slidering parameters for the interface.

