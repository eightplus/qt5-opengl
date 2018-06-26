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
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLTexture>
#include <QTime>

#include "shader.h"

class MainGLWindow : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit MainGLWindow();
    ~MainGLWindow();

protected:
    //在通常情况下，我们还需要实现三个从父类QOpenGLWindow继承的虚函数
    //-----------start------------
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);//若无需对高、宽进行处理，此函数可省
    virtual void paintGL();
    //-----------end------------

private:
    Shader *ourShader;
    QOpenGLFunctions_3_3_Core *core;
    QOpenGLTexture *texture1;
    QOpenGLTexture *texture2;
    QTime time;
};

#endif // MAINGLWINDOW_H
