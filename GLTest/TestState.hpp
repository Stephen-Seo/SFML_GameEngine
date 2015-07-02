
#ifndef TEST_STATE_HPP
#define TEST_STATE_HPP

#include <engine/state.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class TestState : public State
{
public:
    TestState(StateStack& stack, Context context);

    virtual bool update(sf::Time dt, Context context) override;
    virtual void draw(Context context) override;
    virtual bool handleEvent(const sf::Event& event, Context context) override;

private:
    GLuint vao;

};

#endif

