TARGET = opengl14
TEMPLATE = app

#Qt5已将OpenGL集成到了Gui模块中，所以只需包含gui模块就可以了，无需包含opengl
QT += widgets gui

CONFIG += c++11

QMAKE_CPPFLAGS *= $(shell dpkg-buildflags --get CPPFLAGS)
QMAKE_CFLAGS   *= $(shell dpkg-buildflags --get CFLAGS)
QMAKE_CXXFLAGS *= $(shell dpkg-buildflags --get CXXFLAGS)
QMAKE_LFLAGS   *= $(shell dpkg-buildflags --get LDFLAGS)

target.source += $$TARGET
target.path = /usr/bin
INSTALLS += target

unix {
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}

HEADERS += \
    mainglwindow.h \
    shader.h

SOURCES += \
    main.cpp \
    mainglwindow.cpp \
    shader.cpp

RESOURCES += \
    shader.qrc
