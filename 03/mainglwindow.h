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
#include <QOpenGLFunctions>

class MainGLWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MainGLWindow(QWidget *parent = 0);
    ~MainGLWindow();

    float getRadius();
    void setRadius(const float radius);

    void setAfterglowAngle(float afterglowAngle);
    void setScanAngle(float scanAngle);
    void setSuspiciousPoint(const unsigned int distance, const float angle);

    void drawArc();
    void drawRadar();
    void drawCirclePoint(const float centerX,const float centerY, const float radius);

protected:
    //在通常情况下，我们还需要实现三个从父类QOpenGLWindow继承的虚函数
    //-----------start------------
    void initializeGL();
    void resizeGL(int width, int height);//若无需对高、宽进行处理，此函数可省
    void paintGL();
    //-----------end------------

private:
    GLfloat m_radius;//雷达扫描半径
    GLfloat m_sanAngle;//旋转扫描角度
    GLfloat m_afterglowAngle;//余晖角度大小
    unsigned int m_circleCount;//雷达圆圈数
    QPoint m_SuspiciousPoint;//可疑点坐标
};

#endif // MAINGLWINDOW_H
