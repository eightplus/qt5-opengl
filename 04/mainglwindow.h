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

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_3_3_Core>

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

/*
 * Qt对OpenGL的支持，必须public继承QOpenGLWidget类，然后重写这三个函数。
 * 为了使用glShaderSource()这种OpenGL成员函数，要求必须proteced继承QOpenGLFunctions类，或QOpenGLExtraFunctions类，
 * 这里选择继承QOpenGLExtraFunctions类，因为这个类是QOpenGLFunctions类的子类，多了glGenVertexArrays(1, &VAO)这一函数。
 */
//方法1:使用 QOpenGLExtraFunctions
//class MainGLWindow : public QOpenGLWidget, protected QOpenGLExtraFunctions
//{
//    Q_OBJECT

//public:
//    explicit MainGLWindow();
//    ~MainGLWindow();

//protected:
//    //在通常情况下，我们还需要实现三个从父类QOpenGLWindow继承的虚函数
//    //-----------start------------
//    virtual void initializeGL();
//    virtual void resizeGL(int w, int h);//若无需对高、宽进行处理，此函数可省
//    virtual void paintGL();
//    //-----------end------------

//private:
//    GLuint shaderProgram;
//    //QOpenGLFunctions_3_3_Core *core;//方法2:使用 QOpenGLFunctions_3_3_Core
//};


//方法3:使用符合Qt自身编程风格的QOpenGLShader，QOpenGLShaderProgram类，创造Shader类，用QOpenGLFunctions进行数据传送。目前已知，这种基于Qt自身的OpenGL较之原版，缺少顶点数组对象VertexArrayObject，即VAO的功能
class MainGLWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MainGLWindow(QWidget *parent = 0);
    ~MainGLWindow();

protected:
    //在通常情况下，我们还需要实现三个从父类QOpenGLWindow继承的虚函数
    //-----------start------------
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);//若无需对高、宽进行处理，此函数可省
    virtual void paintGL();
    //-----------end------------

private:
    QOpenGLShaderProgram shaderProgram;
    QVector<QVector3D> vertices;
};

#endif // MAINGLWINDOW_H
