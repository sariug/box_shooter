#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
#include <GL/glew.h>
class Shader
{
public:
    Shader();
    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    void CreateFromFile(const char *vertexLocation, const char *fragmentLocation);


    GLuint GetModelProjection();
    GLuint GetModelLocation();
    GLuint GetModelView();

    void UseShader();
    void ClearShader();
    ~Shader();

private:
    std::string ReadFile(const char* fileLocation);
    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint theProgram, const char* shaderCode, GLuint shaderType);
    GLuint shaderID, uniformProjection, uniformModel, uniformView;
};