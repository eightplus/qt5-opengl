#include "shader.h"

Shader::Shader(const QString& vertexPath, const QString& fragmentPath)
{
    QOpenGLShader vertexShader(QOpenGLShader::Vertex);
    bool success = vertexShader.compileSourceFile(vertexPath);
    if(!success) {
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << endl;
        qDebug() << vertexShader.log() << endl;
    }

    QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
    success  =fragmentShader.compileSourceFile(fragmentPath);
    if(!success) {
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << endl;
        qDebug() << fragmentShader.log() << endl;
    }

   shaderProgram.addShader(&vertexShader);
   shaderProgram.addShader(&fragmentShader);
   success = shaderProgram.link();
   if(!success) {
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << endl;
        qDebug() << shaderProgram.log() << endl;
   }
}

Shader::~Shader()
{

}
