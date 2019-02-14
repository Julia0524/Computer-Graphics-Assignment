/*
	Computer Graphics
	Assignment2: Ray Tracing
	Xinyi Yu  516030910278
*/


#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include "Vector3.h"
#include "Object.h"
#include "Ray.h"

using namespace std;

//使用OpenGL显示图像
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawPixels(640, 480, GL_RGBA, GL_FLOAT, Image);
	glFlush();
}


int main(int argc, char **argv) 
{
	vector<Object*> objects;
	//地面
	objects.push_back(&Cube(Vector3(-1000, -4, -80), 2000, 0.1f, 2000, Vector3(0.80, 0.80, 0.80), Vector3(0), 1, 0));
	//球体Sphere
	objects.push_back(&Sphere(Vector3(6, 3, -32), 4, Vector3(1, 0.713, 0.757), Vector3(0), 1, 0)); //light pink sphere (2.55, 1.82, 1.93)
	//立方体Cube
	objects.push_back(&Cube(Vector3(4, -3, -12), 2, 2, 2, Vector3(0.529, 0.808, 0.98),Vector3(0)));
	//模型Model
	Model model("model/bunny_948.off", Vector3(1, 1, 0), Vector3(0));
	//光源
	objects.push_back(&Sphere(Vector3(0, 20, -10), 3, Vector3(0.00, 0.00, 0.00), Vector3(2.5)));
	//渲染
	render(objects, model);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("CG HW2 Julia");
	glClearColor(1, 1, 1, 1);
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}