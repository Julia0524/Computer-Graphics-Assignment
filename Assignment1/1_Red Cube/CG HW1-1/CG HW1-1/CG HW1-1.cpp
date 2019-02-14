/*
CG HW1-1 Red Cube
516030910278 余心怡
*/

#include <GLTools.h>
#include <GL/glut.h>
#include <StopWatch.h>

float rot = 0.0f;
int times = 0;

void drawCube()
{
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);

	//前面
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	//后面
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);

	//顶面
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);

	//底面
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	//右面
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);

	//左面
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);

	glEnd();

	glFlush();
}

void ChangeSize(int w, int h)
{
	if (w > h) {
		glViewport(0, 0, h, h);
	}
	else {
		glViewport(0, 0, w, w);
	}
}

void RenderScene()
{
	times++;
	if (times % 50 == 0) {
		rot += 3.0f;
	}
	if (rot >= 360.0f) {
		rot = 0.0f;
	}

	glEnable(GL_DEPTH_TEST);
	//执行函数进行清除
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	//沿向量(1,1,1)进行角度为rot的旋转
	glRotatef(rot, 1.0f, 1.0f, 1.0f);

	drawCube();
	glPopMatrix();
	glutSwapBuffers();

}

void SetupRC()
{
	//设置用来进行窗口清除的颜色
	glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
}

int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);

	//初始化glut库
	glutInit(&argc, argv);

	//初始化创建窗口的显示模式
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);

	//创建窗口
	glutInitWindowSize(600, 600);
	glutCreateWindow("CG HW1-1 Red Cube");

	//为改变窗口大小设置回调函数以便设置视点
	glutReshapeFunc(ChangeSize);

	//包含OpenGL渲染代码
	glutDisplayFunc(RenderScene);

	glutIdleFunc(RenderScene);

	//检查驱动程序初始化过程中有无出现错误
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error:%s\n", glewGetErrorString(err));
		return 1;
	}
	//调用SetupRC
	SetupRC();

	//开始主消息循环
	glutMainLoop();
	return 0;
}