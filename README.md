qt5-opengl
===========

所有例子均为我个人学习OpenGL所做的记录，源码来自 Joey De Vries 的现代OpenGL教程 和 IHNF(CSDN用户)的博客， 基于Qt5.5及其以上版本进行调试开发，操作系统版本为Ubuntu 16.04。

Qt5已将OpenGL集成到了Gui模块中，所以.pro工程文件中只需包含gui模块就可以了，无需包含opengl，如下：

QT += gui

查看OpenGL版本信息：

$glxinfo | grep OpenGL  (sudo apt install mesa-utils)

若使用Qt但不使用其封装的OpenGL，而是使用 libgl1-mesa-dev 包提供的OpenGL，则在.pro中链接OpenGL库的方式有两种，如下：

1) LIBS += -lGL            (libGL.so)

2) CONFIG += link_pkgconfig

   PKGCONFIG += gl         (gl.pc)


Reference
===========

https://learnopengl-cn.github.io/

https://github.com/LearnOpenGL-CN/LearnOpenGL-CN

https://github.com/JoeyDeVries/LearnOpenGL

https://blog.csdn.net/z136411501/article/details/79832561

http://qiliang.net/old/nehe_qt/index.html

https://github.com/eightplus/qt-opengl
