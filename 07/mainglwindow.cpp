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

GLuint VBO, VAO, EBO;

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
    texture1->destroy();
    texture2->destroy();
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
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
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
    //第一张箱子
    texture1 = new QOpenGLTexture(QImage(":/textures/res/textures/container.jpg").mirrored(), QOpenGLTexture::GenerateMipMaps); //直接生成绑定一个2d纹理, 并生成多级纹理MipMaps
    if(!texture1->isCreated()){
        qDebug() << "Failed to load texture" << endl;
    }
    texture1->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);// 等于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture1->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    texture1->setMinificationFilter(QOpenGLTexture::Linear);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture1->setMagnificationFilter(QOpenGLTexture::Linear);  //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    //第二张笑脸
    texture2 = new QOpenGLTexture(QImage(":/textures/res/textures/smile.png").mirrored(), QOpenGLTexture::GenerateMipMaps); //直接生成绑定一个2d纹理, 并生成多级纹理MipMaps
    if(!texture2->isCreated()){
        qDebug() << "Failed to load texture" << endl;
    }
    texture2->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);// 等于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture2->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    texture2->setMinificationFilter(QOpenGLTexture::Linear);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture2->setMagnificationFilter(QOpenGLTexture::Linear);  //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //设置纹理单元编号
    ourShader->use();
    ourShader->shaderProgram->setUniformValue(ourShader->shaderProgram->uniformLocation("texture1"), 0);//等价于ourShader.setInt("texture1", 0) setInt函数，是Vries自写的函数，实际应用还是要先获取“texture1”的location，然后设值
    ourShader->shaderProgram->setUniformValue(ourShader->shaderProgram->uniformLocation("texture2"), 1);

    core->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    time.start();//开启计时器，返回毫秒
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

    core->glActiveTexture(GL_TEXTURE0);
    texture1->bind();
    core->glActiveTexture(GL_TEXTURE1);
    texture2->bind();

    QMatrix4x4 transform;
    transform.translate(QVector3D(0.5f, -0.5f, 0.0f));
    transform.rotate((float)time.elapsed()/10, QVector3D(0.0f, 0.0f, 1.0f));//elapsed（）返回自start()函数开始的毫秒ms

    ourShader->use();
    ourShader->shaderProgram->setUniformValue(ourShader->shaderProgram->uniformLocation("transform"), transform);
    core->glBindVertexArray(VAO);
    core->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    update();
}
