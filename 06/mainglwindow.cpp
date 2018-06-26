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
    this->setWindowTitle("Texture");
}

MainGLWindow::~MainGLWindow()
{
    delete ourShader;
    core->glDeleteVertexArrays(1, &VAO);
    core->glDeleteBuffers(1, &VBO);
    core->glDeleteBuffers(1, &EBO);
    texture->destroy(); //纹理使用完 进行删除
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
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right //注意新的数据，有纹理单元
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    GLuint indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    core->glGenVertexArrays(1, &VAO);//两个参数，第一个为需要创建的缓存数量。第二个为用于存储单一ID或多个ID的GLuint变量或数组的地址
    core->glGenBuffers(1, &VBO);
    core->glGenBuffers(1, &EBO);

    core->glBindVertexArray(VAO);

    core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    core->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    core->glEnableVertexAttribArray(0);
    // color attribute
    core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    core->glEnableVertexAttribArray(1);
    // texture coord attribute
    core->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    core->glEnableVertexAttribArray(2);

    //纹理
    texture = new QOpenGLTexture(QImage(":/textures/res/textures/container.jpg").mirrored(), QOpenGLTexture::GenerateMipMaps); //直接生成绑定一个2d纹理, 并生成多级纹理MipMaps
    if(!texture->isCreated()){
        qDebug() << "Failed to load texture" << endl;
    }
    texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);// 等于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    texture->setMinificationFilter(QOpenGLTexture::Linear);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);  //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture->setFormat(QOpenGLTexture::RGBFormat); //将纹理储存为rgb值


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
    core->glViewport(0, 0, (GLint)w, (GLint)h);//配置当前窗口大小
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
    texture->bind();
    ourShader->use();
    core->glBindVertexArray(VAO);
    core->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
