/*
CG HW1-3 Car
516030910278 余心怡
*/

#include <GLTools.h>
#include <GL/glut.h>
#include <GLShaderManager.h>
#include <math.h>
#include <iostream>

const GLfloat Pi = 3.1415926536f;

GLBatch triangleBatch;
GLBatch rectangleBatch;
GLBatch circleBatch;
GLShaderManager shaderManager;

/*
小车类
可根据给定小车尺寸参数构造小车
*/
class Car 
{
public:
	GLfloat triBlockSize; //三角形边长的一半
	GLfloat rectBlockSize1; //长方形的长
	GLfloat rectBlockSize2; //长方形的宽
	GLfloat cirBlockSize; //圆形半径
	GLfloat cirX, cirY; //圆心坐标
	GLfloat triVerts[9] = { 0.0f };
	GLfloat rectVerts[12] = { 0.0f };

	Car(GLfloat tri, GLfloat rect1, GLfloat rect2, GLfloat cir);
	//void SetupRC();
	void drawCircle(GLfloat R, GLfloat x, GLfloat y);
	//void SpecialKeys(int key, int x, int y);
	//void ChangeSize(int w, int h);
	//void RenderScene();
};

Car::Car(GLfloat tri, GLfloat rect1, GLfloat rect2, GLfloat cir)
{
	triBlockSize = tri; //三角形边长的一半
	rectBlockSize1 = rect1; //长方形的长
    rectBlockSize2 = rect2; //长方形的宽
	cirBlockSize = cir; //圆形半径
	cirX = -triBlockSize + cirBlockSize, cirY = -rectBlockSize2; //圆心坐标

	triVerts[1] = triBlockSize;
	triVerts[3] = triBlockSize;
	triVerts[6] = -triBlockSize;

	rectVerts[0] = -rectBlockSize1;
	rectVerts[3] = rectBlockSize1;
	rectVerts[6] = rectBlockSize1;
	rectVerts[7] = -rectBlockSize2;
	rectVerts[9] = -rectBlockSize1;
	rectVerts[10] = -rectBlockSize2;
}

/*
绘制圆形
参数：半径R，圆心坐标x，y
*/
void Car::drawCircle(GLfloat R, GLfloat x, GLfloat y)
{
	int n = 2000;
	glBegin(GL_POLYGON);    
	for (int i = 0; i < n; ++i) {
		glVertex2f(x + R*cos(2 * Pi / n*i) , y + R*sin(2 * Pi / n*i));
	}
	glEnd();        
	glFlush();
}

Car c(0.2f, 0.3f, 0.15f, 0.07f);

void SpecialKeys(int key, int x, int y)
{
	GLfloat stepSize = 0.025f;
	GLfloat blockX = c.rectVerts[0]; //长方形左边界
	GLfloat blockY = c.triVerts[1]; //三角形上边界

	if (key == GLUT_KEY_UP) {
		blockY += stepSize;
	}
	if (key == GLUT_KEY_DOWN) {
		blockY -= stepSize;
	}
	if (key == GLUT_KEY_LEFT) {
		blockX -= stepSize;
	}
	if (key == GLUT_KEY_RIGHT) {
		blockX += stepSize;
	}

	//长方形左边界超出屏幕左边
	if (blockX < -1.0f) {
		blockX = -1.0f;
	}
	//长方形右边界超出屏幕右边
	if (blockX > (1.0f - c.rectBlockSize1 * 2)) {
		blockX = 1.0f - c.rectBlockSize1 * 2;
	}
	//三角形上边界超出屏幕上边
	if (blockY > 1.0f) {
		blockY = 1.0f;
	}
	//圆形下边界超出屏幕下边
	if (blockY < (-1.0f +c.triBlockSize + c.cirBlockSize + c.rectBlockSize2)) {
		blockY = -1.0f + c.triBlockSize + c.cirBlockSize + c.rectBlockSize2;
	}

	//三角形顶点变化
	c.triVerts[0] = blockX + c.rectBlockSize1;
	c.triVerts[1] = blockY;

	c.triVerts[3] = blockX + c.rectBlockSize1 + c.triBlockSize;
	c.triVerts[4] = blockY - c.triBlockSize;

	c.triVerts[6] = blockX + c.rectBlockSize1 - c.triBlockSize;
	c.triVerts[7] = blockY - c.triBlockSize;

	//长方形顶点变化
	c.rectVerts[0] = blockX;
	c.rectVerts[1] = blockY - c.triBlockSize;

	c.rectVerts[3] = blockX + c.rectBlockSize1 * 2;
	c.rectVerts[4] = blockY - c.triBlockSize;

	c.rectVerts[6] = blockX + c.rectBlockSize1 * 2;
	c.rectVerts[7] = blockY - c.triBlockSize - c.rectBlockSize2;

	c.rectVerts[9] = blockX;
	c.rectVerts[10] = blockY - c.triBlockSize - c.rectBlockSize2;

	//圆形圆心变化
	c.cirX = blockX + c.rectBlockSize1 - c.triBlockSize + c.cirBlockSize;
	c.cirY = blockY - c.triBlockSize - c.rectBlockSize2;

	triangleBatch.CopyVertexData3f(c.triVerts);
	rectangleBatch.CopyVertexData3f(c.rectVerts);

	//更新窗口内容
	glutPostRedisplay();
}

void ChangeSize(int w, int h)
{
	glViewport(0, 0, w, h);
}

void RenderScene()
{
	//执行函数进行清除
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//以蓝色渲染三角形
	GLfloat vBlue[] = { 0.0f,0.0f,1.0f,1.0f };
	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vBlue);
	triangleBatch.Draw();

	//以红色渲染长方形
	GLfloat vRed[] = { 1.0f,0.0f,0.0f,1.0f };
	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
	rectangleBatch.Draw();

	//以绿色和黄色渲染圆形
	GLfloat vGreen[] = { 0.0f,1.0f,0.0f,1.0f };
	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vGreen);
	c.drawCircle(c.cirBlockSize, c.cirX, c.cirY);

	GLfloat vYellow[] = { 1.0f,1.0f,0.0f,1.0f };
	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vYellow);
	c.drawCircle(c.cirBlockSize, c.cirX + c.triBlockSize * 2 - c.cirBlockSize * 2 , c.cirY);

	glutSwapBuffers();
	glutPostRedisplay();
}

void SetupRC()
{
	//设置用来进行窗口清除的颜色
	glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
	//编译并连接着色器
	shaderManager.InitializeStockShaders();

	//建立三角形的批次
	triangleBatch.Begin(GL_TRIANGLES, 3);
	triangleBatch.CopyVertexData3f(c.triVerts);
	triangleBatch.End();

	//建立长方形的批次
	rectangleBatch.Begin(GL_TRIANGLE_FAN, 4);
	rectangleBatch.CopyVertexData3f(c.rectVerts);
	rectangleBatch.End();

	glFlush();
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
	glutCreateWindow("CG HW1-3 Car");

	//为改变窗口大小设置回调函数以便设置视点
	glutReshapeFunc(ChangeSize);

	//包含OpenGL渲染代码
	glutDisplayFunc(RenderScene);

	glutSpecialFunc(SpecialKeys);
	
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