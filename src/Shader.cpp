#include "Shader.h"

Shader::Shader()
{
    shaderID = 0;
    uniformProjection = 0;
    uniformModel = 0;
}
void Shader::CreateFromString(const char *vertexCode, const char *fragmentCode)
{
    CompileShader(vertexCode, fragmentCode);
}
void Shader::CreateFromFile(const char *vertexLocation, const char *fragmentLocation)
{
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);
    const char *vertexCode = vertexString.c_str();
    const char *fragCode = fragmentString.c_str();
    CompileShader(vertexCode,fragCode);
}

std::string Shader::ReadFile(const char *fileLocation)
{
    std::string content;
    std::ifstream FileStream(fileLocation, std::ios::in);

    if (!FileStream.is_open())
    {
        std::cout << "no file could be opened in " << fileLocation << "\n";
        return "";
    }

    std::string line = "";
    while (!FileStream.eof())
    {
        std::getline(FileStream, line);
        content.append(line + "\n");
    }
    FileStream.close();
    return content;
}

GLuint Shader::GetModelProjection()
{
    return uniformProjection;
}
GLuint Shader::GetModelLocation()
{
    return uniformModel;
}
GLuint Shader::GetModelView(){return uniformView;}
void Shader::UseShader()
{
    glUseProgram(shaderID);
}
void Shader::ClearShader()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }
    uniformModel = 0;
    uniformProjection = 0;
}
Shader::~Shader()
{
    ClearShader();
}

void Shader::CompileShader(const char *vertexCode, const char *fragmentCode)
{
    shaderID = glCreateProgram();

    if (!shaderID)
    {
        std::cout << "Shader program compilation has failed!" << std::endl;
        return;
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint _result = 0;
    GLchar _eCode[1024] = {0};

    glLinkProgram(shaderID);

    glGetProgramiv(shaderID, GL_LINK_STATUS, &_result);
    if (!_result)
    {
        glGetProgramInfoLog(shaderID, sizeof(_eCode), NULL, _eCode);
        std::cout << "Error happened while linking program " << _eCode << std::endl;
        return;
    }

    glValidateProgram(shaderID);

    glGetProgramiv(shaderID, GL_LINK_STATUS, &_result);
    if (!_result)
    {
        glGetProgramInfoLog(shaderID, sizeof(_eCode), NULL, _eCode);
        std::cout << "Error happened while validating program " << _eCode << std::endl;
        return;
    }

    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformView = glGetUniformLocation(shaderID, "view");
}
void Shader::AddShader(GLuint theProgram, const char *shaderCode, GLuint shaderType)
{
    GLuint s = glCreateShader(shaderType);
    const GLchar *c[1];
    c[0] = shaderCode;

    GLint l[1];
    l[0] = strlen(shaderCode);
    glShaderSource(s, 1, c, l);
    glCompileShader(s);

    GLint _result = 0;
    GLchar _eCode[1024] = {0};

    glGetShaderiv(s, GL_COMPILE_STATUS, &_result);
    if (!_result)
    {
        glGetShaderInfoLog(s, sizeof(_eCode), NULL, _eCode);
        std::cout << "Error happened while compuling " << shaderType << " shader" << _eCode << std::endl;
        return;
    }

    glAttachShader(theProgram, s);
}
