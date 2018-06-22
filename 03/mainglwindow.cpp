/*
 * Copyright (C) 2018 lixiang
 *
 * Authors:
 *  lixiang kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainglwindow.h"

#include <QApplication>
#include <QDebug>
#include <GL/glu.h> // for gluPerspective

namespace {
const static float PI = 3.1415926;
const static GLfloat translateZ = -6000.0;
}

MainGLWindow::MainGLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_radius(2000)
    , m_afterglowAngle(60)
    , m_circleCount(5)
    , m_sanAngle(0)
    , m_SuspiciousPoint(QPoint(0,0))
{
    this->resize(QSize(480,500));
}

MainGLWindow::~MainGLWindow()
{

}

/***********************************************************
Function:       initializeGL
Description:    用来初始化当前的OpenGL环境。在此函数中，必须调用initializeOpenGLFunctions()让Qt在后台完成OpenGL环境的初始化工作。
                然后调用glClearColor函数来设置清除颜色。该函数的前三个参数分别是红、绿、蓝的分量值，第四个参数为透明度值。
Calls:
Called By:
Input:
Output:
Return:
Others:
 ************************************************************/
void MainGLWindow::initializeGL()
{
    initializeOpenGLFunctions();//初始化当前OpenGL环境

    glShadeModel(GL_SMOOTH);//设置阴影平滑过度
    glClearColor(0., 0., 0., 0.);//设置背景颜色
    glClearDepth(1.0);//设置好缓冲深度
    glEnable(GL_DEPTH_TEST);//启用深度测试之后，OpenGL在绘制的时候会检查当前像素前面是否有别的像素，如果别的像素挡住了它，则它就不会绘制，即OpenGL只绘制最前面的一层
    glEnable(GL_LINE_SMOOTH);
    glDepthFunc(GL_LEQUAL);//指定深度缓冲比较值
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//指定颜色和纹理坐标的差值质量，选择最高质量选项


    /*
     * void glDepthFunc(GLenum func);
     * 指定“目标像素与当前像素在z方向上值大小比较”的函数，符合该函数关系的目标像素才进行绘制，否则对目标像素不予绘制
     * 通过目标像素与当前像素在z方向上值大小的比较是否满足参数指定的条件，来决定在深度（z方向）上是否绘制该目标像素。
     * 该函数只有启用“深度测试”时才有效，参考glEnable(GL_DEPTH_TEST)和glDisable(GL_DEPTH_TEST)
     * 参数：
     *  func指定深度比较函数，GL_NEVER,GL_LESS,GL_EQUAL,GL_LEQUAL,GL_GREATER,GL_NOTE_QUAL,GL_GEQUAL,GL_ALWAYS,缺省值GL_LESS
     *  GL_NEVER,不通过（输入的深度值不取代参考值）
     *  GL_LESS,如果输入的深度值小于参考值，则通过
     *  GL_EQUAL,如果输入的深度值等于参考值，则通过
     *  GL_LEQUAL,如果输入的深度值小于或等于参考值，则通过
     *  GL_GREATER,如果输入的深度值大于参考值，则通过
     *  GL_NOTE_QUAL,如果输入的深度值不等于参考值，则通过
     *  GL_GEQUAL,如果输入的深度值大于或等于参考值，则通过
     *  GL_ALWAYS,总是通过（输入的深度值取代参考值）
     */

    /*
     * void glHint(GLenum target,GLenum mod)  该函数控制OpenGL在某一方面有解释的余地时，所采取的操作行为
     * 参数：
     * target：指定所控制行为的符号常量，可以是以下值
     *  GL_FOG_HINT：指定雾化计算的精度。如果OpenGL实现不能有效的支持每个像素的雾化计算，则GL_DONT_CARE和GL_FASTEST雾化效果中每个定点的计算。
     *  GL_LINE_SMOOTH_HINT：指定反走样线段的采样质量。如果应用较大的滤波函数，GL_NICEST在光栅化期间可以生成更多的像素段。
     *  GL_PERSPECTIVE_CORRECTION_HINT：指定颜色和纹理坐标的差值质量。如果OpenGL不能有效的支持透视修正参数差值，那么GL_DONT_CARE 和 GL_FASTEST可以执行颜色、纹理坐标的简单线性差值计算。
     *  GL_POINT_SMOOTH_HINT：指定反走样点的采样质量，如果应用较大的滤波函数，GL_NICEST在光栅化期间可以生成更多的像素段。
     *  GL_POLYGON_SMOOTH_HINT：指定反走样多边形的采样质量，如果应用较大的滤波函数，GL_NICEST在光栅化期间可以生成更多的像素段。
     * mod：指定所采取行为的符号常量，可以是以下值
     *  GL_FASTEST：选择速度最快选项。
     *  GL_NICEST：选择最高质量选项。
     *  GL_DONT_CARE：对选项不做考虑。
     */
}

/***********************************************************
Function:       paintGL
Description:    绘制一般在此函数内进行
Calls:
Called By:
Input:
Output:
Return:
Others:
 ************************************************************/
void MainGLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//使用initializeGL()函数中用glClearColor()设置的背景颜色来清除屏幕和深度缓冲

    glLoadIdentity();//加载单位矩阵: 重置当前观察模型矩阵，将当前点移到屏幕中心
    //画坐标系
    glTranslatef(0.0, 0.0, translateZ);//平移变换，三个参数分别为沿x,y,z轴方向的平移分量

    //绘制横竖坐标线
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f( -m_radius, 0.0f);
    glVertex2f( m_radius, 0.0f);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f( 0.0f, -m_radius);
    glVertex2f( 0.0f, m_radius);
    glEnd();

    drawArc();

    setSuspiciousPoint(2000, 5);//设置可疑点1

    drawRadar();//绘制雷达余晖

    setSuspiciousPoint(1600, 1);//设置可疑点2
}

float MainGLWindow::getRadius()
{
    return m_radius;
}

/***********************************************************
Function:       setRadius
Description:    设置扫描半径
Calls:
Called By:
Input:
Output:
Return:
Others:
 ************************************************************/
void MainGLWindow::setRadius(const float radius)
{
    m_radius = radius;
}

/***********************************************************
Function:       setAfterglowAngle
Description:    余晖角度-机器扫描角度
Calls:
Called By:
Input:
Output:
Return:
Others:
 ************************************************************/
void MainGLWindow::setAfterglowAngle(float afterglowAngle)
{
    m_afterglowAngle = afterglowAngle;
}

/***********************************************************
Function:       setScanAngle
Description:    扫描角度，内部已经添加负号，顺时针旋转数为正值，逆时针为负值
Calls:
Called By:
Input:
Output:
Return:
Others:
 ************************************************************/
void MainGLWindow::setScanAngle(float scanAngle)
{
    m_sanAngle = -scanAngle;
}

/***********************************************************
Function:       resizeGL
Description:    主要用来对高度和宽度进行一些变化处理。无特殊需求时，不处理即可
Calls:
Called By:
Input:
Output:
Return:
Others:
 ************************************************************/
void MainGLWindow::resizeGL(int width, int height)
{
    if(height == 0)
        height = 1;

    glViewport(0,0,(GLint)width,(GLint)height);//配置当前窗口大小
    glMatrixMode(GL_PROJECTION);//选择投影矩阵
    glLoadIdentity();//加载单位矩阵: 重置投影矩阵

    /*
    void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
    fovy是眼睛上下睁开的幅度，角度值，值越小，视野范围越狭小（眯眼），值越大，视野范围越宽阔（睁开铜铃般的大眼）；
    aspect表示裁剪面的宽w高h比，这个影响到视野的截面有多大
    zNear表示近裁剪面到眼睛的距离，zFar表示远裁剪面到眼睛的距离，注意zNear和zFar不能设置设置为负值。
    */
    gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.1, 10000.0);//建立透视矩阵: 视场是在纵向 0.1-10000之间，你眼前就是最近的，最远的就是负值
    glMatrixMode(GL_MODELVIEW);//选择模型观察矩阵
    glLoadIdentity();//加载单位矩阵: 重置模型观察矩阵
}

//画圆点  centerX,centerY为输入的圆心坐标
void MainGLWindow::drawCirclePoint(const float centerX,const float centerY, const float radius)
{
    int n = 500;

    /*
    glColor3f(0.0, 0.0, 0.0);  --> 黑色
    glColor3f(1.0, 0.0, 0.0);  --> 红色
    glColor3f(0.0, 1.0, 0.0);  --> 绿色
    glColor3f(0.0, 0.0, 1.0);  --> 蓝色
    glColor3f(1.0, 1.0, 0.0);  --> 黄色
    glColor3f(1.0, 0.0, 1.0);  --> 品红色
    glColor3f(0.0, 1.0, 1.0);  --> 青色
    glColor3f(1.0, 1.0, 1.0);  --> 白色
    */

    glBegin(GL_POLYGON);

    glColor3f(1.0, 1.0, 0.0);//设置颜色为黄色
    for (int i =0;i<n;i++) {
        glVertex2f(centerX+radius*cos(2*PI/n*i), centerY+radius*sin(2*PI/n*i));//画点
    }

    glEnd();
}

/***********************************************************
Function:       drawArc
Description:    画雷达的所有圆弧圈圈
Calls:
Called By:
Input:
Output:
Return:
Others:
 ************************************************************/
void MainGLWindow::drawArc()
{
    GLfloat r = 0.01;
    int n = 500;

    glColor3f(0.0,1.0,0.0);//设置颜色为绿色

    for (int i =0; i<m_circleCount; i++) {
        r = m_radius/m_circleCount + (m_radius/m_circleCount )*i;

        glBegin(GL_LINE_LOOP);//GL_POLYGON:画实心圆
        for (int j =0;j<n;j++) {
            glVertex2f(r*cos(2*PI*j/n), r*sin(2*PI*j/n));
        }
        glEnd();
    }
}

/***********************************************************
Function:       drawRadar
Description:    绘制雷达余晖
Calls:
Called By:
Input:
Output:
Return:
Others:
 ************************************************************/
void MainGLWindow::drawRadar()
{
    GLfloat green = 1.0;

    glEnable(GL_BLEND);//启用混合
    glPushMatrix();
    glRotatef(m_sanAngle, 0.0f,0.0f,1.0f);//第一个参数是旋转的角度，后三个参数是向量，围绕这个向量旋转
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_LINES);
    for (float f=0;f<180;f=f+0.1) {//雷达余晖算法
        green = 0.9-(f/180);
        glColor4f(0, green, 0.0, 0.4-(f/180));//红色 绿色 蓝色 透明值
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(m_radius*cos(PI*((30.0+m_sanAngle)+f)/180.0), m_radius*sin(PI*((30.0+m_sanAngle)+f)/180.0), 0.0f);
    }
    glEnd();

    glPopMatrix();
    glDisable(GL_BLEND);//关闭混合

    if (m_sanAngle > -360)
        m_sanAngle -= 0.1;
    else
        m_sanAngle=0;

    update();
}

/***********************************************************
Function:       drawRadar
Description:    setSuspiciousPoint
Calls:
Called By:
Input:
Output:
Return:
Others:
 ************************************************************/
void MainGLWindow::setSuspiciousPoint(const unsigned int distance, const float angle)
{
    unsigned int pointX = 0;
    unsigned int pointY = 0;

    if (angle > 360 || angle < 0 || distance > m_radius || distance < 0)
        return;

    pointX = distance*sin(angle * PI/180);
    pointY = distance*cos(angle * PI/180);

    m_SuspiciousPoint.setX(pointX);
    m_SuspiciousPoint.setY(pointY);

    drawCirclePoint(m_SuspiciousPoint.x(), m_SuspiciousPoint.y(), 30);
}
