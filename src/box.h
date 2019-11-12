#include "Mesh.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
const float toRadians = glm::pi<float>() / 180.0f;

// Vertex Shader
static const char *vShader = R"(../shaders/shader.vert)";

// Pixel Shader
static const char *fShader = R"(../shaders/shader.frag)";

class Box
{
public:
    Box(GLfloat x, GLfloat y, GLfloat z)
    {

        origin = glm::vec3(x, y, z);
        //model = glm::translate(model, origin);
        this->createBoxMesh();

        createShader(vShader, fShader);
    }
    Box()
    {
        origin = glm::vec3(0, 0, 0);
        this->createBoxMesh();
    }
    ~Box() { _mesh->clearMesh(); }
    void scaleBox(glm::vec3 _scale)
    {
        scale = _scale;
    }
    void createShader(const char *vertexLocation, const char *fragmentLocation)
    {
        _shader = new Shader;
        _shader->CreateFromFile(vertexLocation = vShader, fragmentLocation = fShader);
    }

    glm::mat4& set_transformation(){return model;}
    void draw(glm::mat4 projection, glm::mat4 cameraView)
    {

        _shader->UseShader();
        uniformModel = _shader->GetModelLocation();
        uniformProjection = _shader->GetModelProjection();
        uniformView = _shader->GetModelView();
        model = glm::scale(model, scale);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(cameraView));


        _mesh->renderMesh();
        glUseProgram(0);
    }
    glm::vec3 getOrigin()
    {
        return origin;
    }

private:
    void createBoxMesh()
    {
        unsigned int indices[]{
            // front
            0, 1, 2,
            2, 3, 0,
            // right
            1, 5, 6,
            6, 2, 1,
            // back
            7, 6, 5,
            5, 4, 7,
            // left
            4, 0, 3,
            3, 7, 4,
            // bottom
            4, 5, 1,
            1, 0, 4,
            // top
            3, 2, 6,
            6, 7, 3};
        GLfloat vertices[] = {
            // front
            -1.0, -1.0, 1.0 ,
            1.0, -1.0 , 1.0,
            1.0, 1.0, 1.0,
            -1.0, 1.0, 1.0,
            // back
            -1.0, -1.0, -1.0,
            1.0, -1.0, -1.0,
            1.0, 1.0, -1.0,
            -1.0, 1.0, -1.0};

        _mesh = new Mesh();

        _mesh->createMesh(vertices, indices, 24, 36);
    }
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    GLuint uniformModel, uniformProjection, uniformView;
    glm::vec3 origin;
    Mesh *_mesh;
    Shader *_shader;
    glm::mat4 model;
};