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
#include <QOpenGLShader>
#include <QScreen>

//着色器语言
static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

//着色器语言
static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";


MainGLWindow::MainGLWindow(QWindow *parent)
    : QWindow(parent)
    , m_context(0)
    , m_frame(0)
    , m_update_pending(false)
{
    resize(500, 680);
    setSurfaceType(QWindow::OpenGLSurface);
}

MainGLWindow::~MainGLWindow()
{
    if(m_context)
        delete m_context;
}

void MainGLWindow::doDrawWork()
{
    if(m_context) {
        m_context->makeCurrent(this);
        drawImage();
        m_context->swapBuffers(this);
        return;
    }

    m_context = new QOpenGLContext(this);
    m_context->create();

    m_context->makeCurrent(this);

    initializeOpenGLFunctions();

    initShader();
    drawImage();

    m_context->swapBuffers(this);
}

void MainGLWindow::initShader()
{
    m_program = new QOpenGLShaderProgram(this);


/*
    //顶点着色器 vertexshader.vert
    //vPosition和vColor是用来承接三角形的位置和颜色数据的；matrix是模型矩阵，可以用来进行移动、旋转等变化；
    //fColor是顶点着色器和片段着色器间的交互数据；gl_Position和gl_FrgColor是OpenGL内部识别变量
    if(!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "vertexshader.vert"))
        return;

    //片段着色器 fragmentshader.frag
    if(!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "fragmentshader.frag"))
        return;*/

    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
}

void MainGLWindow::drawImage()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);
    matrix.rotate(100.0f * m_frame++ / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    GLfloat vertices[] = {
        0.0f, 0.707f,
        -0.5f, -0.5f,
        0.5f, -0.5f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    m_program->release();
}

bool MainGLWindow::event(QEvent *event)
{
    if(event->type() != QEvent::UpdateRequest)
        return QWindow::event(event);

    m_update_pending = false;
    if(isExposed()) run();

    return true;
}

//每次窗口大小改变是都会产生exposeEvent信号，绘画结束后通过postEvent产生一个event信号
void MainGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if(isExposed()) run();
}

void MainGLWindow::run()
{
    doDrawWork();

    if (m_update_pending) return;
    m_update_pending = true;

    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}




/*GLuint verVbo;// VBO标示符
GLuint colorVbo;// VBO标示符
GLuint indexVbo;// VBO标示符

GLsizeiptr triIndexLen;

//三角形顶点坐标
GLfloat vertex[] = {
    -0.5f, 0.0f,
    0.5f, 0.0f,
    0.0f, 0.5f,
};

//三角形顶点索引
GLuint triIndexs[] = {0, 1, 2};

//三角形顶点颜色
GLfloat colors[] = {1.0f, 0.0f, 0.0f, 1.0f,
                    0.0f, 1.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f, 1.0f,
                   };


MainGLWindow::MainGLWindow(QWindow *parent)
    : QWindow(parent)
{
    resize(500, 680);
}

MainGLWindow::~MainGLWindow()
{

}

void MainGLWindow::initVbo()
{
    // 计算获得数组长度
    GLsizeiptr vVerticesLen = sizeof(vertex)/sizeof(GLfloat);
    triIndexLen = sizeof(triIndexs)/sizeof(GLuint);
    GLsizeiptr colorsLen = sizeof(colors)/sizeof(GLfloat);

    // 初始化顶点buffer并装载数据到显存
    glGenBuffers(1, &verVbo);
    glBindBuffer(GL_ARRAY_BUFFER, verVbo);
    glBufferData(GL_ARRAY_BUFFER, vVerticesLen * sizeof(GLfloat), vertex, GL_STATIC_DRAW);

    // 初始化索引buffer并装载数据到显存
    glGenBuffers(1, &indexVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triIndexLen * sizeof(GLuint), triIndexs, GL_STATIC_DRAW);

    // 初始化颜色buffer并装载数据到显存
    glGenBuffers(1, &colorVbo);
    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glBufferData(GL_ARRAY_BUFFER, colorsLen * sizeof(GLfloat), colors, GL_STATIC_DRAW);
}

void MainGLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    m_program = new QOpenGLShaderProgram(this);


    //顶点着色器 vertexshader.vert
    //vPosition和vColor是用来承接三角形的位置和颜色数据的；matrix是模型矩阵，可以用来进行移动、旋转等变化；
    //fColor是顶点着色器和片段着色器间的交互数据；gl_Position和gl_FrgColor是OpenGL内部识别变量
    if(!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "vertexshader.vert"))
        return;

    //片段着色器 fragmentshader.frag
    if(!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "fragmentshader.frag"))
        return;

    if(!m_program->link())
        return;

    if(!m_program->bind())
        return;

    // 获取shaderprogram的id号，然后可以通过id号获取一些属性...
    int programId = m_program->programId();

    // 从shaderprogram里面获取变量标识
    m_matrixUniform = glGetUniformLocation(programId, "matrix");//获取指向着色器中matrix的index
    m_posAttr = glGetAttribLocation(programId, "vPosition");//获取指向着色器中vPosition的index
    m_colAttr = glGetAttribLocation(programId, "vColor");//获取指向着色器中vColor的index

    this->initVbo();
}

void MainGLWindow::resizeGL(int w, int h)
{
//    glViewport(0, 0, w, h);

//    // 模型矩阵重置
//    projection.setToIdentity();
//    // 透视投影
//    qreal aspect = qreal(w) / qreal(h ? h : 1);
//    projection.perspective(60.0f, aspect, 1.0f, 100.0f);
//    // 增加了模型矩阵，需要做一定偏移量，保证物体刚开始渲染出来时可以被看到！
//    projection.translate(0.0f, 0.0f, -2.0f);
}

void MainGLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // shader传入模型视图矩阵 projection可以理解为建立了一个坐标系空间，可以再这个空间内设置图形
//    glUniformMatrix4fv(m_matrixUniform, 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(m_matrixUniform, 1, GL_FALSE, 0);

    // shader绑定并启用颜色数组buffer
    glBindBuffer(GL_ARRAY_BUFFER,colorVbo);
    glVertexAttribPointer(m_colAttr, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_colAttr);
    // 颜色值rgba，所以每四个float值作为一个颜色值，如果只是希望rgb，取三个值作为颜色值即可!


    // shader绑定并启用顶点数组buffer
    glBindBuffer(GL_ARRAY_BUFFER, verVbo);
    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_posAttr);

    // shader绑定并顶点索引数组buffer - 索引无需启用
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexVbo);
    glDrawElements(GL_TRIANGLES, triIndexLen, GL_UNSIGNED_INT,0);

    // 解绑buffer、关闭启用顶点、颜色数组
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(m_posAttr);
    glDisableVertexAttribArray(m_colAttr);
}*/
