#ifndef SHADER_H
#define SHADER_H

#include <QDebug>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QString>

class Shader {
public:
  Shader(const QString& vertexSourcePath, const QString& fragmentSourcePath);
  ~Shader();
  QOpenGLShaderProgram *shaderProgram;

  void use(){
    shaderProgram->bind();
  }

  //还是把设置着色器uniform变量操作写成Shader里的inline成员函数管理，真的方便很多。
  void setMat4(const QString& name, const QMatrix4x4& value){
      GLuint loc = shaderProgram->uniformLocation(name);
      shaderProgram->setUniformValue(loc, value);
  }

  void setInt(const QString& name, const GLint& value){
      GLuint loc = shaderProgram->uniformLocation(name);
      shaderProgram->setUniformValue(loc, value);
  }
};

#endif // SHADER_H
