#include <gl\glut.h>    
#include <math.h>
#include <windows.h>
#include <stdio.h>
#include<cmath>
#include<iostream>

const int Width = 600;
const int Height = 600;

const GLfloat PI = 3.141592653f;

// 当前时间，时 分 秒
float h = 0.0f;
float m = 0.0f;
float s = 0.0f;

void setPixel(int x, int y)
{

    glVertex2i(x, y);
}

/*
    8路对称
*/
void Cirpot(int x0, int y0, int x, int y)
{
    // 1
    setPixel((x0 + x), (y0 + y));
    // 2
    setPixel((x0 + y), (y0 + x));
    // 3
    setPixel((x0 + y), (y0 - x));
    // 4
    setPixel((x0 + x), (y0 - y));
    // 5
    setPixel((x0 - x), (y0 - y));
    // 6
    setPixel((x0 - y), (y0 - x));
    // 7
    setPixel((x0 - y), (y0 + x));
    // 8
    setPixel((x0 - x), (y0 + y));
}

/*
    中点画圆算法
*/
void MidPoint_Circle(int x0, int y0, int r)
{
    int x = 0;
    int y = r;
    float d = 1.25 - r;     
    Cirpot(x0, y0, x, y);
    while (x <= y)
    {
        if (d < 0)
        {
            d += 2 * x + 3;
        }
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
        Cirpot(x0, y0, x, y);
    }
}


/*
画时钟的函数
*/
void myDisplay(void)
{
	// 用当前清除色清除颜色缓冲区，即设定窗口的背景色
    
    //glColor3f(0.1f, 0.2f, 0.1f); // 黑色
    int cx = Width / 2; //中心点
    int cy = Height / 2;
    int R = 100; // 半径长
    int i; 

    // 2 绘制时钟 圆上的刻度，刻度粗细，长度不一样
    int lines = 60;
    float ship1 = 10;
    float ship2 = 5;
    for (i = 0; i<lines; i++)
    {
        if (i % 5 == 0)    //  0（12）-11的短线
        { 
            glLineWidth(5);
            //glColor3f(0.0, 1.0, 0.0);
            glBegin(GL_LINES);
            glVertex2f(cx + (R - ship1)*sin(2 * PI / lines * i), cy + (R - ship1)*cos(2 * PI / lines * i));
            glVertex2f(cx + R*sin(2 * PI / lines * i), cy + R*cos(2 * PI / lines * i));
            glEnd();
        }
        else
        {
            glLineWidth(2);  //其他的刻度
            //glColor3f(0.0, 0.0, 1.0);
            glBegin(GL_LINES);
            glVertex2f(cx + (R - ship2)*sin(2 * PI / lines * i), cy + (R - ship2)*cos(2 * PI / lines * i));
            glVertex2f(cx + R*sin(2 * PI / lines * i), cy + R*cos(2 * PI / lines * i));
            glEnd();
        }
    }

    // 3 绘制时 分 秒结合的中心的小黑点
    glBegin(GL_POLYGON);
    for (i = 0; i<100; ++i)
        glVertex2i(cx + 5 * cos(2 * PI / 100*i), cy + 5 * sin(2 * PI / 100*i));
    glEnd();

    // 4 绘制时 分 秒针，三条直线，长短粗细不一样
    int h_len = R - 3 * ship1;
    int m_len = R - ship1;
    int s_len = R - ship1 + 2 * ship1;

    float s_Angle = s / 60.0;
    float m_Angle = (m * 60 + s) / 3600.0;
    float h2 = h >= 12 ? (h - 12) : h;
    float h_Angle = (h2 * 60 * 60 + m * 60 + s) / (12 * 60 * 60);

    glLineWidth(1); // 秒
    glBegin(GL_LINES);
    glVertex2f(cx - 2 * ship1 * sin(2 * PI*s_Angle), cy - 2 * ship1*cos(2 * PI*s_Angle));
    glVertex2f(cx + (R - ship2)*sin(2 * PI*s_Angle), cy + (R - ship2)*cos(2 * PI*s_Angle));
    glEnd();

    glLineWidth(5); // 时
    glBegin(GL_LINES);
    glVertex2f(cx, cy);
    glVertex2f(cx + h_len*sin(2 * PI*h_Angle), cy + h_len*cos(2 * PI*h_Angle));
    glEnd();

    glLineWidth(3); // 分
    glBegin(GL_LINES);
    glVertex2f(cx, cy);
    glVertex2f(cx + m_len*sin(2 * PI*m_Angle), cy + m_len*cos(2 * PI*m_Angle));
    glEnd();

    // 刷新
    glFlush();
}

/*
回调函数
*/
void timerFunc(int value)
{
    s += 1;
    int carry1 = 0;
    if (s >= 60)
    {
        s = 0;
        carry1 = 1;
    }
    m += carry1;
    int carry2 = 0;
    if (m >= 60)
    {
        m = 0;
        carry2 = 1;
    }
    h += carry2;
    if (h >= 24)
        h = 0;

    glutPostRedisplay(); // 重画

    glutTimerFunc(1000, timerFunc, 1); // 每一秒中执行一次
}

/*
初始化函数
设置背景，获取时间
*/
void SetupRC(void)
{
    // 设置窗口为白色
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // 获取本地当前时间
    SYSTEMTIME sys;
    GetLocalTime(&sys);
    h = sys.wHour;
    m = sys.wMinute;
    s = sys.wSecond;
}

void reshapse(GLsizei w, GLsizei h)//调整窗口形状不发生改变
{  
	 w=w>h?h:w;  
	 glViewport(0, 0, w, h);  
}  

void display()
{	
	int cx = Width / 2; //中心点
    int cy = Height / 2;
    int R = 100; // 半径长
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
	MidPoint_Circle(cx,cy,R);
	glEnd();


	glBegin(GL_LINES);
	myDisplay();
	glEnd();

	glFlush();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//设置窗口左上角的位置   
    glutInitWindowSize(Width, Height);
    glutCreateWindow("clock");
    //观察参数：x坐标值从0到200，y是从0到150  
    gluOrtho2D(0.0, Width, 0.0, Height);
	SetupRC();

	glutReshapeFunc(reshapse);//窗口大小改变保证图形不产生畸变
    glutDisplayFunc(display);
	
	glutTimerFunc(1000, timerFunc, 1);

    glutMainLoop(); // glut事件处理循环
	
    return 0;
}
