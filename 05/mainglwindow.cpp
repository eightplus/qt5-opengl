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

GLuint VBO, VAO;

MainGLWindow::MainGLWindow()
{

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
    //使用QOpenGLFunctions_3_3_core核心类对象，作为数据传输工具，因为QOpenGLFunctions类没有glGenVertexArray()函数，即没有顶点数组对象这一概念
    //着色器部分
    core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    ourShader = new Shader(":/shader/vertexshadersource.vert", ":/shader/fragmentshadersource.frag");

    //VAO，VBO数据部分
    GLfloat vertices[] = {
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom Left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top
    };

    core->glGenVertexArrays(1, &VAO);//两个参数，第一个为需要创建的缓存数量。第二个为用于存储单一ID或多个ID的GLuint变量或数组的地址
    core->glGenBuffers(1, &VBO);
    core->glBindVertexArray(VAO);
    core->glBindBuffer(GL_ARRAY_BUFFER, VBO);

    core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    core->glEnableVertexAttribArray(0);
    core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    core->glEnableVertexAttribArray(1);

    core->glBindBuffer(GL_ARRAY_BUFFER, 0);
    core->glBindVertexArray(0);

    core->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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
void MainGLWindow::resizeGL(int w, int h)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, (GLint)w, (GLint)h);//配置当前窗口大小
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
    core->glClear(GL_COLOR_BUFFER_BIT);
    ourShader->use();
    core->glBindVertexArray(VAO);
    core->glDrawArrays(GL_TRIANGLES, 0, 3);
}
