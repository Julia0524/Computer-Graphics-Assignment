/*
CG HW1-3 Car
516030910278 ������
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
С����
�ɸ��ݸ���С���ߴ��������С��
*/
class Car 
{
public:
	GLfloat triBlockSize; //�����α߳���һ��
	GLfloat rectBlockSize1; //�����εĳ�
	GLfloat rectBlockSize2; //�����εĿ�
	GLfloat cirBlockSize; //Բ�ΰ뾶
	GLfloat cirX, cirY; //Բ������
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
	triBlockSize = tri; //�����α߳���һ��
	rectBlockSize1 = rect1; //�����εĳ�
    rectBlockSize2 = rect2; //�����εĿ�
	cirBlockSize = cir; //Բ�ΰ뾶
	cirX = -triBlockSize + cirBlockSize, cirY = -rectBlockSize2; //Բ������

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
����Բ��
�������뾶R��Բ������x��y
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
	GLfloat blockX = c.rectVerts[0]; //��������߽�
	GLfloat blockY = c.triVerts[1]; //�������ϱ߽�

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

	//��������߽糬����Ļ���
	if (blockX < -1.0f) {
		blockX = -1.0f;
	}
	//�������ұ߽糬����Ļ�ұ�
	if (blockX > (1.0f - c.rectBlockSize1 * 2)) {
		blockX = 1.0f - c.rectBlockSize1 * 2;
	}
	//�������ϱ߽糬����Ļ�ϱ�
	if (blockY > 1.0f) {
		blockY = 1.0f;
	}
	//Բ���±߽糬����Ļ�±�
	if (blockY < (-1.0f +c.triBlockSize + c.cirBlockSize + c.rectBlockSize2)) {
		blockY = -1.0f + c.triBlockSize + c.cirBlockSize + c.rectBlockSize2;
	}

	//�����ζ���仯
	c.triVerts[0] = blockX + c.rectBlockSize1;
	c.triVerts[1] = blockY;

	c.triVerts[3] = blockX + c.rectBlockSize1 + c.triBlockSize;
	c.triVerts[4] = blockY - c.triBlockSize;

	c.triVerts[6] = blockX + c.rectBlockSize1 - c.triBlockSize;
	c.triVerts[7] = blockY - c.triBlockSize;

	//�����ζ���仯
	c.rectVerts[0] = blockX;
	c.rectVerts[1] = blockY - c.triBlockSize;

	c.rectVerts[3] = blockX + c.rectBlockSize1 * 2;
	c.rectVerts[4] = blockY - c.triBlockSize;

	c.rectVerts[6] = blockX + c.rectBlockSize1 * 2;
	c.rectVerts[7] = blockY - c.triBlockSize - c.rectBlockSize2;

	c.rectVerts[9] = blockX;
	c.rectVerts[10] = blockY - c.triBlockSize - c.rectBlockSize2;

	//Բ��Բ�ı仯
	c.cirX = blockX + c.rectBlockSize1 - c.triBlockSize + c.cirBlockSize;
	c.cirY = blockY - c.triBlockSize - c.rectBlockSize2;

	triangleBatch.CopyVertexData3f(c.triVerts);
	rectangleBatch.CopyVertexData3f(c.rectVerts);

	//���´�������
	glutPostRedisplay();
}

void ChangeSize(int w, int h)
{
	glViewport(0, 0, w, h);
}

void RenderScene()
{
	//ִ�к����������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//����ɫ��Ⱦ������
	GLfloat vBlue[] = { 0.0f,0.0f,1.0f,1.0f };
	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vBlue);
	triangleBatch.Draw();

	//�Ժ�ɫ��Ⱦ������
	GLfloat vRed[] = { 1.0f,0.0f,0.0f,1.0f };
	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
	rectangleBatch.Draw();

	//����ɫ�ͻ�ɫ��ȾԲ��
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
	//�����������д����������ɫ
	glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
	//���벢������ɫ��
	shaderManager.InitializeStockShaders();

	//���������ε�����
	triangleBatch.Begin(GL_TRIANGLES, 3);
	triangleBatch.CopyVertexData3f(c.triVerts);
	triangleBatch.End();

	//���������ε�����
	rectangleBatch.Begin(GL_TRIANGLE_FAN, 4);
	rectangleBatch.CopyVertexData3f(c.rectVerts);
	rectangleBatch.End();

	glFlush();
}

int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);

	//��ʼ��glut��
	glutInit(&argc, argv);

	//��ʼ���������ڵ���ʾģʽ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);

	//��������
	glutInitWindowSize(600, 600);
	glutCreateWindow("CG HW1-3 Car");

	//Ϊ�ı䴰�ڴ�С���ûص������Ա������ӵ�
	glutReshapeFunc(ChangeSize);

	//����OpenGL��Ⱦ����
	glutDisplayFunc(RenderScene);

	glutSpecialFunc(SpecialKeys);
	
	//������������ʼ�����������޳��ִ���
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error:%s\n", glewGetErrorString(err));
		return 1;
	}
	//����SetupRC
	SetupRC();

	//��ʼ����Ϣѭ��
	glutMainLoop();
	return 0;
}