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

#ifndef MAINGLWINDOW_H
#define MAINGLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

/*
为方面使用OpenGL函数，从QOpenGLWindow继承的同时又同时从QOpenGLFunctions继承，当前QT版本中，不需要GLEW，
因为QOpenGLFunctions可以访问OpenGL ES 2.0 API。也可以不从QOpenGLFunctions继承，通过QOpenGLContext来访问OpenGL函数，
用法有2种:
QOpenGLFunctions functions(QOpenGLContext::currentContext());
functions.glFunctionHere();

or

QOpenGLFunctions *functions = QOpenGLContext::currentContext()->functions();
functions->glFunctionHere();
*/
class MainGLWindow : public QOpenGLWidget, protected QOpenGLFunctions
//class MainGLWindow : public QWindow, protected QOpenGLFunctions
//class MainGLWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MainGLWindow(QWidget *parent = 0);//for QOpenGLWidget
//    explicit MainGLWindow(QWindow *parent = 0);//for QWindow
    ~MainGLWindow();

    void printContextInformation();

    //在通常情况下，我们还需要实现三个从父类QOpenGLWindow继承的虚函数
    //-----------start------------
    void initializeGL();
    void resizeGL(int width, int height);//若无需对高、宽进行处理，此函数可省
    void paintGL();
    //-----------end------------

private:

};

#endif // MAINGLWINDOW_H
