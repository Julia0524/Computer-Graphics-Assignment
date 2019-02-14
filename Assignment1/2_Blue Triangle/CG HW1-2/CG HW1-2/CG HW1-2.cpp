/*
CG HW1-2 Blue Triangle
516030910278 余心怡
*/

#include <GLTools.h>
#include <GL/glut.h>
#include <GLShaderManager.h>

GLBatch triangleBatch;
GLShaderManager shaderManager;

GLfloat vVerts[] = {
	0.0f,0.5f,0.0f,
	0.5f,-0.5f,0.0f,
	-0.5f,-0.5f,0.0f
};

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
	
	glutSwapBuffers();

}

void SetupRC()
{
	//设置用来进行窗口清除的颜色
	glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
	//编译并连接着色器
	shaderManager.InitializeStockShaders();

	//建立三角形的批次
	triangleBatch.Begin(GL_TRIANGLES, 3);
	triangleBatch.CopyVertexData3f(vVerts);
	triangleBatch.End();
}

int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);

	//初始化glut库
	glutInit(&argc, argv);

	//初始化创建窗口的显示模式
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);

	//创建窗口
	glutInitWindowSize(800, 600);
	glutCreateWindow("CG HW1-2 Blue Triangle");

	//为改变窗口大小设置回调函数以便设置视点
	glutReshapeFunc(ChangeSize);

	//包含OpenGL渲染代码
	glutDisplayFunc(RenderScene);

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