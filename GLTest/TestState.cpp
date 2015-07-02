
#include "TestState.hpp"

#include <cassert>

#include <iostream>

#include <engine/GLHelper.hpp>

TestState::TestState(StateStack& stack, Context context) :
State(stack, context)
{
    assert(glGetError() == GL_NO_ERROR);

/*
    float ratio;
    int width, height;
    glfwGetFramebufferSize(context.window, &width, &height);

    assert(glGetError() == GL_NO_ERROR);

    ratio = width / (float) height;
    glViewport(0, 0, width, height);

    assert(glGetError() == GL_NO_ERROR);

    glClear(GL_COLOR_BUFFER_BIT);

    assert(glGetError() == GL_NO_ERROR);

    glMatrixMode(GL_PROJECTION);

    GLenum error = glGetError();
    std::cout << (error == GL_INVALID_ENUM) << ' ' << (error == GL_INVALID_OPERATION) << std::endl;
    assert(error == GL_NO_ERROR);

    glLoadIdentity();

    assert(glGetError() == GL_NO_ERROR);

    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);

    assert(glGetError() == GL_NO_ERROR);

    glMatrixMode(GL_MODELVIEW);

    assert(glGetError() == GL_NO_ERROR);

    glLoadIdentity();

    assert(glGetError() == GL_NO_ERROR);
*/

    GLHelper::ElementManager& elementManager = GLHelper::ElementManager::getInstance();

    vao = elementManager.addVAO();
    std::vector<GLfloat> vertices = {
        -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    };
/*
    std::vector<GLfloat> vertices = {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        100.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 100.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
    };
*/
    std::vector<GLuint> elements = {
        0, 1, 2,
        2, 3, 0
    };

    elementManager.setupVAO(vao, vertices, elements);
}

bool TestState::update(sf::Time dt, Context context)
{
#ifdef GAME_THREADED_DRAW
    std::cout << dt.asSeconds() << std::endl;
#endif //GAME_THREADED_DRAW
    return false;
}

void TestState::draw(Context context)
{
    GLHelper::ElementManager::getInstance().draw();
}

bool TestState::handleEvent(const sf::Event& event, Context context)
{
    return false;
}
