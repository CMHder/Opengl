#include <iostream>
#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>
 
#define SOLID 1
#define WIRE 2
 
int moveX,moveY;
int spinX = 0;
int spinY = 0;
int des = 0;
 
 
void init() {   //创建、定位和启用光源
   //定义光源的颜色和位置
   GLfloat ambient[] = { 0.5, 0.8, 0.1, 0.1 };  
   GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };  
   GLfloat position[] = { -80.0, 50.0, 25.0, 1.0 };  
   
   //选择光照模型
   GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };  
   GLfloat local_view[] = { 0.0 };  
   glClearColor(0.0, 0.0, 0.0, 0.0);   //视口背景：低调黑色
  glShadeModel(GL_FLAT);  
   
   //创建光源
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);  //设置环境光
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);  //设置漫射光
   glLightfv(GL_LIGHT0, GL_POSITION, position);  //设置光源位置
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);  
   glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);  
  
   //启动光照和光源
   glEnable(GL_LIGHTING);   //启动光照
   glEnable(GL_LIGHT1);   //启用一号光源
}
 
//画球
void drawBall(double R, double x, double y,double z, int MODE) {
    glPushMatrix();
    glTranslated(x,y,z);
    if (MODE == SOLID) {
        glutSolidSphere(R,30,30);
    } 
	else if (MODE ==WIRE) {
        glutWireSphere(R,20,20);
    }
    glPopMatrix();
}
 
//画半球
void drawHalfBall(double R, double x, double y,double z, int MODE) {
    glPushMatrix();
    glTranslated(x,y,z);
    GLdouble eqn[4]={0.0, 1.0, 0.0, 0.0};
    glClipPlane(GL_CLIP_PLANE0,eqn);  
    glEnable(GL_CLIP_PLANE0);  //启用图形切割管道,与glDisable相对应
    if (MODE == SOLID) {
        glutSolidSphere(R,20,20);
    } 
	else if (MODE ==WIRE) {
        glutWireSphere(R,20,20);
    }
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
}
 
//画长方体
void drawSkewed(double l, double w, double h, double x, double y, double z, int MODE) {
    glPushMatrix();
    glScaled(l, w, h);   //glScaled(10.0f,10.0f,10.0f)表示将当前图形沿x,y,z轴分别放大为原来的10倍
    glTranslated(x, y, z);
    if (MODE == SOLID) {
        glutSolidCube(1);
    } 
	else if (MODE ==WIRE) {
        glutWireCube(1);
    }
    glPopMatrix();
}
 
void display(void) {
    //清除缓冲区颜色
    glClear(GL_COLOR_BUFFER_BIT);
    //定义白色
    glColor3f(1.0, 1.0, 1.0); 
    //圆点放坐标中心
    glLoadIdentity();

    //从哪个地方看
    gluLookAt(-2.0, -1.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);  //该函数定义一个视图矩阵，并与当前矩阵相乘。
	//相当于我们的脑袋位置在(-2.0,-1.0,20.0)处，眼睛望向(0.0,0.0,0.0),即原点。后面的三个参数(0.0,1.0,0.0),y轴为1，其余为0，表示脑袋朝上，就是正常的情况。 

    glPushMatrix();
    glRotated(spinX, 0, 1, 0); //glRotatef( angle, x, y, z )函数的作用是将当前坐标系以a( x, y, z )向量为旋转轴旋转angle角度。
    glRotated(spinY, 1, 0, 0);
    glTranslated(0, 0, des);
    
    drawBall(2, 0, 0, 0, SOLID);  //头
   
    drawSkewed(5, 4, 4, 0, -1, 0, SOLID);  //身体
    
    drawHalfBall(1, 3.5, -2, 0, SOLID);  //肩膀
    drawHalfBall(1, -3.5, -2, 0, SOLID);
    
    drawSkewed(1, 3, 1, 3.5, -1.3, 0, SOLID);  //胳膊
    drawSkewed(1, 3, 1, -3.5, -1.3, 0, SOLID);
    
    drawBall(1, 3.5, -6.4, 0, SOLID);  //手
    drawBall(1, -3.5, -6.4, 0, SOLID);
    
    drawSkewed(1.2, 3, 2, 1, -2.4, 0, SOLID);  //腿
    drawSkewed(1.2, 3, 2, -1, -2.4, 0, SOLID);
 
    drawSkewed(1.5, 1, 3, 0.9, -9.2, 0, SOLID);  //脚
    drawSkewed(1.5, 1, 3, -0.9, -9.2, 0, SOLID);

    glPopMatrix();
    glutSwapBuffers();
}

//鼠标点击事件
void mouseClick(int btn, int state, int x, int y) {
    moveX = x;
    moveY = y;
    GLfloat ambient[] = { (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 0.1 }; 
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);    //设置环境光 
    glEnable(GL_LIGHT0);  //启用光源
}
 
//键盘事件
void keyPressed(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
        spinX -= 2;
        break;
    case 'd':
         spinX += 2;  
        break;
    case 'w':
        des += 2;
        break;
    case 's':
        des -= 2;
        break;
    }
    glutPostRedisplay();
}

// 鼠标移动事件 
void mouseMove(int x, int y) {
    int dx = x - moveX;
    int dy = y - moveY;
    printf("dx;%dx,dy:%dy\n",dx,dy);
    spinX += dx;
    spinY += dy;
    glutPostRedisplay();  //glutPostRedisplay函数会标记当前窗体来重新显示,它会促使主循环尽快的调用完显示函数.    
	moveX = x;
    moveY = y;
}
 
void reshape(int w, int h) {
    
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);  //定义视口大小
    glMatrixMode(GL_PROJECTION);  //投影显示
    glLoadIdentity();  //坐标原点在屏幕中心
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);  //操作模型视景
    glMatrixMode(GL_MODELVIEW);
}
 
int main(int argc, char** argv) {
   
    glutInit(&argc, argv);  //初始化
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);   //设置显示模式
    glutInitWindowSize(500, 500);  //初始化窗口大小
    glutInitWindowPosition(100, 100);  //定义左上角窗口位置
    glutCreateWindow(argv[0]);  //创建窗口
    init();    //初始化
    glutDisplayFunc(display);  //显示函数

    glutReshapeFunc(reshape);  //窗口大小改变时的响应
   
    glutMouseFunc(mouseClick);   //鼠标点击事件，鼠标点击或者松开时调用
    glutMotionFunc(mouseMove); //鼠标移动事件，"鼠标按下"并移动时调用
    glutKeyboardFunc(keyPressed);  //键盘事件
 
    glutMainLoop(); //循环
    return 0;
}
