
#ifdef GAME_USE_GLFW

#include "GLHelper.hpp"

#include <cassert>

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

GLHelper::ElementManager::~ElementManager()
{
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgramWithTexture);
}

GLHelper::ElementManager& GLHelper::ElementManager::getInstance()
{
    static ElementManager instance;
    return instance;
}

GLHelper::ElementManager::ElementManager()
{
    const GLchar* vertexSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 position;"
        "layout(location = 1) in vec4 color;"
        "out vec4 Color;"
        "void main() {"
        "   Color = color;"
        "   gl_Position = vec4(position, 1.0);"
        "}";

    const GLchar* fragmentSource =
        "#version 330 core\n"
        "in vec4 Color;"
        "out vec4 outColor;"
        "void main() {"
        "   outColor = Color;"
        "}";
/*
    const GLchar* fragmentSource =
        "#version 330 core\n"
        "in vec4 Color;"
        "out vec4 outColor;"
        "void main() {"
        "   outColor = vec4(1.0, 1.0, 1.0, 1.0);"
        "}";
*/

    const GLchar* vertexSourceWithTexture =
        "#version 330 core\n"
        "layout(location = 0) in vec3 position;"
        "layout(location = 1) in vec4 color;"
        "layout(location = 2) in vec2 texcoord;"
        "out vec4 Color;"
        "out vec2 Texcoord;"
        "void main() {"
        "    Color = color;"
        "    Texcoord = texcoord;"
        "    gl_Position = vec4(position, 1.0);"
        "}";

    const GLchar* fragmentSourceWithTexture =
        "#version 330 core\n"
        "in vec4 Color;"
        "in vec2 Texcoord;"
        "out vec4 outColor;"
        "uniform sampler2D tex;"
        "void main() {"
        "    outColor = texture(tex, Texcoord) * Color;"
        "}";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint vertexShaderWithTexture = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint fragmentShaderWithTexture = glCreateShader(GL_FRAGMENT_SHADER);

    assert(glGetError() == GL_NO_ERROR);

    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLint result;
    int infoLogLength;

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0)
    {
        std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(vertexShader, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
        std::cout << &vertexShaderErrorMessage[0] << std::endl;
    }

    assert(glGetError() == GL_NO_ERROR);

    glShaderSource(vertexShaderWithTexture, 1, &vertexSourceWithTexture, NULL);
    glCompileShader(vertexShaderWithTexture);

    glGetShaderiv(vertexShaderWithTexture, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderWithTexture, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0)
    {
        std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(vertexShaderWithTexture, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
        std::cout << &vertexShaderErrorMessage[0] << std::endl;
    }

    assert(glGetError() == GL_NO_ERROR);

    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0)
    {
        std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
        std::cout << &vertexShaderErrorMessage[0] << std::endl;
    }

    assert(glGetError() == GL_NO_ERROR);

    glShaderSource(fragmentShaderWithTexture, 1, &fragmentSourceWithTexture, NULL);
    glCompileShader(fragmentShaderWithTexture);

    glGetShaderiv(fragmentShaderWithTexture, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderWithTexture, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0)
    {
        std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(fragmentShaderWithTexture, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
        std::cout << &vertexShaderErrorMessage[0] << std::endl;
    }

    assert(glGetError() == GL_NO_ERROR);

    shaderProgram = glCreateProgram();
    shaderProgramWithTexture = glCreateProgram();

    assert(glGetError() == GL_NO_ERROR);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    assert(glGetError() == GL_NO_ERROR);

    glAttachShader(shaderProgramWithTexture, vertexShaderWithTexture);
    glAttachShader(shaderProgramWithTexture, fragmentShaderWithTexture);

    assert(glGetError() == GL_NO_ERROR);

//    glBindFragDataLocation(shaderProgram, 0, "outColor");
//    glBindFragDataLocation(shaderProgramWithTexture, 0, "outColor");

    assert(glGetError() == GL_NO_ERROR);

    glLinkProgram(shaderProgram);
    glLinkProgram(shaderProgramWithTexture);

    assert(glGetError() == GL_NO_ERROR);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShaderWithTexture);
    glDeleteShader(fragmentShaderWithTexture);

    assert(glGetError() == GL_NO_ERROR);
}

GLHelper::ElementManager::Element::Element() :
texture(nullptr),
isReady(false)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

GLHelper::ElementManager::Element::~Element()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

GLuint GLHelper::ElementManager::addVAO()
{
    Element::Ptr ptr(new Element());
    GLuint vao = ptr->vao;
    elementMap.insert(std::make_pair(vao, std::move(ptr)));

    return vao;
}

void GLHelper::ElementManager::removeVAO(GLuint vao)
{
    elementMap.erase(vao);
}

void GLHelper::ElementManager::setupVAO(GLuint vao, std::vector<GLfloat> vertices, std::vector<GLuint> elements, const sf::Texture* texture, bool isDynamic)
{
    Element* element = elementMap.at(vao).get();

    assert(!element->isReady &&
        "ERROR: Element already isReady!");

    unsigned int verticesCount;
    if(texture == nullptr)
    {
        assert(vertices.size() % 7 == 0 &&
            "ERROR: Invalid vertices size to have color coordinates!");
        verticesCount = vertices.size() / 7;
    }
    else
    {
        assert(vertices.size() % 9 == 0 &&
            "ERROR: Invalid vertices size to have color and texture coordinates!");
        verticesCount = vertices.size() / 9;
        element->texture = texture;
    }

    assert(verticesCount > 2 &&
        "ERROR: Element only has at most two complete vertices!");

    assert(elements.size() % 3 == 0 &&
        "ERROR: Invalid number of element indices!");

    element->drawIndices = elements.size();

    glBindVertexArray(element->vao);
    glBindBuffer(GL_ARRAY_BUFFER, element->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), (isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));

    if(element->texture == nullptr)
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    }
    else
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * elements.size(), elements.data(), GL_STATIC_DRAW);

//    glBindVertexArray(0);

    element->isReady = true;
}

void checkError(std::string line)
{
    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::cout << line << ' ';
        switch(error)
        {
        case GL_INVALID_ENUM:
            std::cout << "GL_INVALID_ENUM\n";
            break;
        case GL_INVALID_VALUE:
            std::cout << "GL_INVALID_VALUE\n";
            break;
        case GL_INVALID_OPERATION:
            std::cout << "GL_INVALID_OPERATION\n";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION\n";
            break;
        case GL_OUT_OF_MEMORY:
            std::cout << "GL_OUT_OF_MEMORY\n";
            break;
        default:
            std::cout << "UNKNOWN GL ERROR\n";
            break;
        }
    }
}

void GLHelper::ElementManager::draw()
{
    checkError("start draw");
    for(auto elementIter = elementMap.begin(); elementIter != elementMap.end(); ++elementIter)
    {
        if(!elementIter->second->isReady)
        {
#ifndef NDEBUG
            std::cout << "Skipping element that is not ready...\n";
#endif
            continue;
        }

        if(elementIter->second->texture != nullptr)
        {
            glUseProgram(shaderProgramWithTexture);

            sf::Texture::bind(elementIter->second->texture);
        }
        else
        {
            glUseProgram(shaderProgram);
            checkError("glUseProgram");
        }

        glBindVertexArray(elementIter->second->vao);

        checkError("glBindVertexArray");
/*
        glBindBuffer(GL_ARRAY_BUFFER, elementIter->second->vbo);
        if(elementIter->second->texture == nullptr)
        {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        }
        else
        {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementIter->second->ebo);
*/

        glDrawElements(GL_TRIANGLES, elementIter->second->drawIndices, GL_UNSIGNED_INT, 0);
        checkError("glDrawElements");

//        glBindVertexArray(0);

        if(elementIter->second->texture != nullptr)
        {
            sf::Texture::bind(NULL);
        }
    }
}

#endif

