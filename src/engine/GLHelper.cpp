
#include "GLHelper.hpp"

#include <cassert>

#include <iostream>

#include <SFML/Graphics.hpp>

GLHelper::ElementManager::~ElementManager()
{
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgramWithTexture);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShaderWithTexture);
    glDeleteShader(fragmentShaderWithTexture);
}

GLHelper::ElementManager& GLHelper::ElementManager::getInstance()
{
    static ElementManager instance;
    return instance;
}

GLHelper::ElementManager::ElementManager()
{
    const GLchar* vertexSource =
        "#version 150 core\n"
        "in vec3 position;"
        "in vec4 color;"
        "out vec4 Color;"
        "void main() {"
        "   Color = color;"
        "   gl_Position = vec4(position, 1.0);"
        "}";

    const GLchar* fragmentSource =
        "#version 150 core\n"
        "in vec4 Color;"
        "out vec4 outColor;"
        "void main() {"
        "   outColor = Color;"
        "}";

    const GLchar* vertexSourceWithTexture =
        "#version 150 core\n"
        "in vec3 position;"
        "in vec4 color;"
        "in vec2 texcoord;"
        "out vec4 Color;"
        "out vec2 Texcoord;"
        "void main() {"
        "    Color = color;"
        "    Texcoord = texcoord;"
        "    gl_Position = vec4(position, 1.0);"
        "}";

    const GLchar* fragmentSourceWithTexture =
        "#version 150 core\n"
        "in vec4 Color;"
        "in vec2 Texcoord;"
        "out vec4 outColor;"
        "uniform sampler2D tex;"
        "void main() {"
        "    outColor = texture(tex, Texcoord) * Color;"
        "}";

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    vertexShaderWithTexture = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShaderWithTexture = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    glShaderSource(vertexShaderWithTexture, 1, &vertexSourceWithTexture, NULL);
    glCompileShader(vertexShaderWithTexture);

    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    glShaderSource(fragmentShaderWithTexture, 1, &fragmentSourceWithTexture, NULL);
    glCompileShader(fragmentShaderWithTexture);

    shaderProgram = glCreateProgram();
    shaderProgramWithTexture = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glAttachShader(shaderProgramWithTexture, vertexShaderWithTexture);
    glAttachShader(shaderProgramWithTexture, fragmentShaderWithTexture);

    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glBindFragDataLocation(shaderProgramWithTexture, 0, "outColor");

    glLinkProgram(shaderProgram);
    glLinkProgram(shaderProgramWithTexture);
}

GLHelper::ElementManager::Element::Element() :
vertexAdded(false),
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

void GLHelper::ElementManager::setupVAO(GLuint vao, std::vector<float> vertices, std::vector<GLuint> elements, const sf::Texture* texture, bool isDynamic)
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

    assert(verticesCount <= 1 &&
        "ERROR: Element only has at most one complete vertex!");

    assert(elements.size() % 3 == 0 &&
        "ERROR: Invalid number of element indices!");

    element->isLine = verticesCount == 2;
    element->drawIndices = elements.size();

    glBindVertexArray(element->vao);
    glBindBuffer(GL_ARRAY_BUFFER, element->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.data()), vertices.data(), (isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements.data()), elements.data(), GL_STATIC_DRAW);

    if(element->texture == nullptr)
    {
        GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);

        GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
        glEnableVertexAttribArray(colAttrib);
        glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    }
    else
    {
        GLint posAttrib = glGetAttribLocation(shaderProgramWithTexture, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);

        GLint colAttrib = glGetAttribLocation(shaderProgramWithTexture, "color");
        glEnableVertexAttribArray(colAttrib);
        glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

        GLint texAttrib = glGetAttribLocation(shaderProgramWithTexture, "texcoord");
        glEnableVertexAttribArray(texAttrib);
        glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));
    }

    glBindVertexArray(0);

    element->isReady = true;
}

void GLHelper::ElementManager::draw()
{
    for(auto elementIter = elementMap.begin(); elementIter != elementMap.end(); ++elementIter)
    {
        if(elementIter->second->texture != nullptr)
        {
            glUseProgram(shaderProgramWithTexture);

            sf::Texture::bind(elementIter->second->texture);
        }
        else
        {
            glUseProgram(shaderProgram);
        }

        glBindVertexArray(elementIter->second->vao);

        glDrawElements(GL_TRIANGLES, elementIter->second->drawIndices, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        if(elementIter->second->texture != nullptr)
        {
            sf::Texture::bind(NULL);
        }
    }
}

