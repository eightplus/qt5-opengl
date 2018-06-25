qt5-opengl
===========

所有例子都是基于Qt5.5进行开发的，采用的操作系统版本为Ubuntu 16.04。
Qt5已将OpenGL集成到了Gui模块中，所以.pro工程文件中只需包含gui模块就可以了，无需包含opengl，如下：
QT += gui

查看OpenGL版本信息：
$glxinfo | grep OpenGL  (sudo apt install mesa-utils)

若使用Qt但不使用其封装的OpenGL，而是使用 libgl1-mesa-dev 包提供的OpenGL，则在.pro中链接OpenGL库的方式有两种，如下：
1)
LIBS += -lGL            (libGL.so)

2)
CONFIG += link_pkgconfig
PKGCONFIG += gl         (gl.pc)


https://learnopengl-cn.github.io/

https://github.com/LearnOpenGL-CN/LearnOpenGL-CN

http://qiliang.net/old/nehe_qt/index.html

https://blog.csdn.net/z136411501/article/details/79832561
