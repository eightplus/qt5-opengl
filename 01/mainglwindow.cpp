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

//for QOpenGLWidget
MainGLWindow::MainGLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{
    resize(500, 680);
}

//for QWindow
/*MainGLWindow::MainGLWindow(QWindow *parent)
    : QWindow(parent)
{
    resize(500, 680);
}*/

MainGLWindow::~MainGLWindow()
{
    //makeCurrent();
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

//    this->printContextInformation();

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);//设置清除颜色
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
    glClear(GL_COLOR_BUFFER_BIT);//使用在函数initializeGL()中设置的清楚颜色值来清除窗口背景
}

/***********************************************************
Function:       printContextInformation
Description:    调试
Calls:
Called By:
Input:
Output:
Return:
Others:
 ************************************************************/
void MainGLWindow::printContextInformation()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile())
    {
        CASE(NoProfile);
        CASE(CoreProfile);
        CASE(CompatibilityProfile);
    }
#undef CASE

    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
