#include "glut.h" 
int board[3][3];   //存储几个方块的颜色
#define BUFSIZE 512
//处理点击记录:
//hits为产生的点击的数量，buffer中存储点击记录，每个点击记录由四个项目组成
void processHits(GLint hits, GLuint buffer[])
{
	unsigned int i, j;
	GLuint ii, jj, names, *ptr;
	ptr = (GLuint *)buffer;

	for (i = 0; i<hits; i++)    //处理每一个点击记录
	{
		//某一个点击记录来说，由四个项目组成：
		//（1）当点击发生时，名字堆栈中的名称数量
		//（2）自上一个点击记录之后，与视景体相交的所有顶点的最小和最大窗口坐标z值
		//（3）当点击发生时，名称堆栈的内容，从最底部的元素开始
		names = *ptr;      //获得名字堆栈中的名称数量
		ptr += 3;               //跳过前三个记录
		for (j = 0; j<names; j++) //开始处理名字堆栈中的内容，获取被选中的方块的index
		{
			//对应于绘制方块时，压入名字堆栈中的内容
			if (j == 0)        //x方向上的index
				ii = *ptr;
			else if (j == 1)  //y方向上的index
				jj = *ptr;
			ptr++;
		}
	}
	board[ii][jj] = (board[ii][jj] + 1) % 3;   //改变被选中方块的颜色
}

//绘制所有方块，参数有GL_RENDER和GL_SELECT两种模式
void drawSquares(GLenum mode)
{
	GLuint i, j;
	for (i = 0; i<3; i++)
	{
		if (mode == GL_SELECT)       //如果是在选择模式下，将名字堆栈的首元素换成x方向上的索引
			glLoadName(i);

		for (j = 0; j<3; j++)
		{
			if (mode == GL_SELECT)    //将y方向上的索引压入名字堆栈
				glPushName(j);

			//绘制方块，在GL_SELECT模式下，某一个方块会被选中，因此，会产生一个点击记录
			//该点击被记录时，名字堆栈中有两个名称，分别是i和j的值，也就是被选中方块的索引
			glColor3f((GLfloat)i / 3.0, (GLfloat)j / 3.0, (GLfloat)board[i][j] / 3.0);
			glRecti(i, j, i + 1, j + 1);

			if (mode == GL_SELECT) //弹出名字
				glPopName();
		}
	}
}

//当鼠标左键点击窗口时，进入选择模式开始绘制；绘制之后，处理点击记录
void pickSquares(int button, int state, int x, int y)
{
	GLuint selectBuf[BUFSIZE];   //存储点击记录
	GLint hits;                                 //点击记录的个数
	GLint viewport[4];                    //视口

	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;

	glGetIntegerv(GL_VIEWPORT, viewport);  //获得视口
	glSelectBuffer(BUFSIZE, selectBuf);    //指定存储点击记录的数组
	glRenderMode(GL_SELECT);          //进入选择模式

	glInitNames();           //初始化名字堆栈并压入初始元素
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	//设置挑选矩阵，挑选区域的中心坐标是(x,viewport[3]-y)，大小是(5,5)
	gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 5.0, 5.0, viewport);
	//设置投影矩阵
	gluOrtho2D(0.0, 3.0, 0.0, 3.0);
	//在选择模式下绘制方块
	drawSquares(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();        //绘制结束

	//处理点击记录
	hits = glRenderMode(GL_RENDER); //获取记录下的点击的个数
	processHits(hits, selectBuf);           //处理点击记录selectBuf
	glutPostRedisplay();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
	for (int i = 0; i <3; i++)            //初始化9个方块的颜色
		for (int j = 0; j<3; j++)
			board[i][j] = 0;
}
void display()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawSquares(GL_RENDER);  //基本绘制
	glFlush();
}
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 3.0, 0.0, 3.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(200, 200);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("pick");
	init();
	glutMouseFunc(pickSquares);   //当鼠标点击时，调用pickSquares，进入选择模式进行绘制
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);      //display只完成基本的绘制
	glutMainLoop();
	return 0;
}