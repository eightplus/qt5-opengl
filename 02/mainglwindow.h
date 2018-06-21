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

#include <QWindow>
#include <QOpenGLFunctions>

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
class QOpenGLShaderProgram;
QT_END_NAMESPACE

/*class MainGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MainGLWindow(QWindow *parent = 0);
    ~MainGLWindow();

    void initVbo();

    //在通常情况下，我们还需要实现三个从父类QOpenGLWindow继承的虚函数
    //-----------start------------
    void initializeGL();
    void resizeGL(int width, int height);//若无需对高、宽进行处理，此函数可省
    void paintGL();
    //-----------end------------

private:

    QOpenGLContext *m_context;              // 画板对象
    QOpenGLShaderProgram *m_program;        // 着色器(画刷)对象
    GLuint m_posAttr;                       // 着色器参数：属性
    GLuint m_colAttr;                       // 着色器参数：纹理
    GLuint m_matrixUniform;                 // 着色器参数：uniform
};*/


class MainGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit MainGLWindow(QWindow *parent = 0);
     ~MainGLWindow();

      void doDrawWork();                  // Test绘图
      void initShader();                    // 初始化着色器(画刷)
      void drawImage();                     // 执行绘图动作
private:
    QOpenGLContext *m_context;              // 画板对象
    QOpenGLShaderProgram *m_program;        // 着色器(画刷)对象

    GLuint m_posAttr;                       // 着色器参数：属性
    GLuint m_colAttr;                       // 着色器参数：纹理
    GLuint m_matrixUniform;                 // 着色器参数：uniform

    int  m_frame;                           // 循环变量，只要到一次
    bool m_update_pending;                  // 绘图完成标志(窗口更新标志)

public slots:
    void run();                             // 动画效果实现

protected:                                  // 消息发生器(重载而来)
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;
};

#endif // MAINGLWINDOW_H
