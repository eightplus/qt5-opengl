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

namespace {
const char *vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main(){\n"
        "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
        "}\n\0";
const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";
}

MainGLWindow::MainGLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{
    this->resize(QSize(480,500));
}

MainGLWindow::~MainGLWindow()
{

}

void MainGLWindow::initializeGL()
{
    this->initializeOpenGLFunctions();//这个init()函数至关重要，如果继承QOpenGlFunctions,必须使用这个初始化函数
    //着色器部分
    QOpenGLShader vertexShader(QOpenGLShader::Vertex);
    bool success = vertexShader.compileSourceCode(vertexShaderSource);
    if(!success) {
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << endl;
        return;
    }

    QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
    success = fragmentShader.compileSourceCode(fragmentShaderSource);
    if(!success) {
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << endl;
        return;
    }

    shaderProgram.addShader(&vertexShader);
    shaderProgram.addShader(&fragmentShader);
    success = shaderProgram.link();
    if(!success) {
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << endl;
    }
    shaderProgram.bind();

    //VAO，VBO数据部分
    vertices.append(QVector3D(-0.5, -0.5, 0.0));
    vertices.append(QVector3D(0.5, -0.5, 0.0));
    vertices.append(QVector3D(0.0, 0.5, 0.0));
}

void MainGLWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void MainGLWindow::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);//使用initializeGL()函数中用glClearColor()设置的背景颜色来清除屏幕和深度缓冲
    shaderProgram.bindAttributeLocation("aPos", 0);//等于glUseProgram(shaderProgram)
    shaderProgram.enableAttributeArray(0);
    shaderProgram.setAttributeArray(0, vertices.data());
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}




//unsigned int VBO, VAO;//GLuint VBO, VAO;
//namespace {
//const char *vertexShaderSource = "#version 330 core\n"
//    "layout (location = 0) in vec3 aPos;\n"
//    "void main()\n"
//    "{\n"
//    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//    "}\0";
//const char *fragmentShaderSource = "#version 330 core\n"
//    "out vec4 FragColor;\n"
//    "void main()\n"
//    "{\n"
//    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//    "}\n\0";
//}

//MainGLWindow::MainGLWindow()
//{

//}

//MainGLWindow::~MainGLWindow()
//{

//}

///***********************************************************
//Function:       initializeGL
//Description:    用来初始化当前的OpenGL环境。在此函数中，必须调用initializeOpenGLFunctions()让Qt在后台完成OpenGL环境的初始化工作。
//                然后调用glClearColor函数来设置清除颜色。该函数的前三个参数分别是红、绿、蓝的分量值，第四个参数为透明度值。
//Calls:
//Called By:
//Input:
//Output:
//Return:
//Others:
// ************************************************************/
//void MainGLWindow::initializeGL()
//{
//    initializeOpenGLFunctions();//初始化当前OpenGL环境

//    //Core类其实是一种Qt对OpenGL特定版本的包装类对象，可以直接使用该版本OpenGL的所有函数
//    //如果使用QOpenGLFunctions_3_3_Core类对象，则代码中所有gl开头的函数都加上core->，如core->glCreateShader
//    //core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();//获取上下文

//    // build and compile our shader program
//    // ------------------------------------
//    // vertex shader
//    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);

//    // check for shader compile errors
//    int success;
//    char infoLog[512];
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success)  {
//        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
//    }
//    // fragment shader
//    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//    // check for shader compile errors
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
//    }
//    // link shaders
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//    // check for linking errors
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
//    }
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);

//    // set up vertex data (and buffer(s)) and configure vertex attributes
//    // ------------------------------------------------------------------
////    GLfloat vertices[] = {
////            -0.5f, -0.5f, 0.0f, // left
////             0.5f, -0.5f, 0.0f, // right
////             0.0f,  0.5f, 0.0f  // top
////    };
//    float vertices[] = {
//        -0.5f, -0.5f, 0.0f, // left
//         0.5f, -0.5f, 0.0f, // right
//         0.0f,  0.5f, 0.0f  // top
//    };

//    glGenVertexArrays(1, &VAO);//两个参数，第一个为需要创建的缓存数量。第二个为用于存储单一ID或多个ID的GLuint变量或数组的地址
//    glGenBuffers(1, &VBO);
//    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
//    glEnableVertexAttribArray(0);

//    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
//    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//    glBindVertexArray(0);
//}

///***********************************************************
//Function:       resizeGL
//Description:    主要用来对高度和宽度进行一些变化处理。无特殊需求时，不处理即可
//Calls:
//Called By:
//Input:
//Output:
//Return:
//Others:
// ************************************************************/
//void MainGLWindow::resizeGL(int w, int h)
//{
//    // make sure the viewport matches the new window dimensions; note that width and
//    // height will be significantly larger than specified on retina displays.
//    glViewport(0, 0, (GLint)w, (GLint)h);//配置当前窗口大小
//}

///***********************************************************
//Function:       paintGL
//Description:    绘制一般在此函数内进行
//Calls:
//Called By:
//Input:
//Output:
//Return:
//Others:
// ************************************************************/
//void MainGLWindow::paintGL()
//{
//    // render
//    // ------
//    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);//使用initializeGL()函数中用glClearColor()设置的背景颜色来清除屏幕和深度缓冲

//    // draw our first triangle
//    glUseProgram(shaderProgram);
//    glBindVertexArray(VAO);// seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    //glBindVertexArray(0);// no need to unbind it every time
//    glUseProgram(0);
//}
