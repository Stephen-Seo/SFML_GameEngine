
#include "game.hpp"

#include <iostream>
#include <cassert>

#ifdef GAME_USE_GLFW
    #include <cmath>

std::vector<sf::Event>* Game::staticEvents = nullptr;
#else
  #ifdef GAME_NO_RENDER_WINDOW
    #include <GL/gl.h>
  #endif
#endif

// set packfile name/filepath if one is being used
#define PACKFILE_NAME ""

// set to true if a packfile is being used
#define IS_USING_PACKFILE false

// if not using cmake to build and using the ResourcePacker lib,
// define ResourcePacker_FOUND


#if defined(ResourcePacker_FOUND)
#else
#  define IS_USING_PACKFILE false
#endif

#if IS_USING_PACKFILE == true
#  define RESOURCE_MANAGER_MODE GameResources::PACKFILE
#else
#  define RESOURCE_MANAGER_MODE GameResources::DEFAULT
#endif

Game::Game()
#ifdef GAME_USE_GLFW
:
#else
: window(sf::VideoMode(960,540), "SFML App", sf::Style::Titlebar | sf::Style::Close),
#endif
resourceManager(&stateStack, RESOURCE_MANAGER_MODE, PACKFILE_NAME),
mPlayer(),
sPlayer(),
stateStack(),
context(
#ifdef GAME_USE_GLFW
window,
#else
&window,
#endif
resourceManager, mPlayer, sPlayer, ecEngine, isQuitting, connection, clearColor),
isQuitting(false),
connection()
{
    frameTime = sf::seconds(1.f / 60.f);

#ifdef GAME_USE_GLFW
    Game::staticEvents = &events;

    glfwSetErrorCallback([] (int error, const char* description) {
        std::cerr << description << std::endl;
    });

    assert(glfwInit());

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(960, 540, "GLFW App", NULL, NULL);
    if(!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, GameCallbacks::keyCallback);

    glfwSetCharCallback(window, GameCallbacks::charCallback);

    glfwSetCursorPosCallback(window, GameCallbacks::cursorPosCallback);

    glfwSetCursorEnterCallback(window, GameCallbacks::cursorEnterCallback);

    glfwSetMouseButtonCallback(window, GameCallbacks::mouseButtonCallback);

    glfwSetScrollCallback(window, GameCallbacks::scrollCallback);
#else
  #ifdef GAME_NO_RENDER_WINDOW
    sf::ContextSettings settings = window.getSettings();

    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 2;

    window.create(sf::VideoMode(960,540), "SFML App", sf::Style::Titlebar | sf::Style::Close, settings);
    #ifndef NDEBUG
    settings = window.getSettings();

    std::cout << "depth bits:" << settings.depthBits << std::endl;
    std::cout << "stencil bits:" << settings.stencilBits << std::endl;
    std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
    std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;
    #endif
  #endif
#endif
}

Game::~Game()
{
#ifdef GAME_USE_GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
#endif
}

void Game::run()
{
    stateStack.pushState(startingState);

    sf::Time lastUpdateTime = sf::Time::Zero;

#ifdef GAME_USE_GLFW
    glfwSetTime(0.0);

    while(!glfwWindowShouldClose(window) && !isQuitting)
    {
        lastUpdateTime = sf::seconds(glfwGetTime());
        glfwSetTime(0.0);
        while(lastUpdateTime > frameTime)
        {
            lastUpdateTime -= frameTime;
            processEvents();
            update(frameTime);
        }
        draw();
    }
#else
    sf::Clock clock;

    while(window.isOpen() && !isQuitting)
    {
        lastUpdateTime += clock.restart();
        while (lastUpdateTime > frameTime)
        {
            lastUpdateTime -= frameTime;
            processEvents();
            update(frameTime);
        }
        draw();
    }
#endif

#ifndef GAME_USE_GLFW
    if(window.isOpen())
        window.close();
#endif
}

void Game::setStartingState(const std::string& stateName)
{
    startingState = stateName;
}

void Game::processEvents()
{
#ifdef GAME_USE_GLFW
    glfwPollEvents();

    if(!events.empty())
    {
        for(unsigned int i = 0; i < events.size(); ++i)
        {
            stateStack.handleEvent(events[i], context);
        }
        events.clear();
    }
#else
    sf::Event event;
    while (window.pollEvent(event))
    {
        stateStack.handleEvent(event, context);
        if(event.type == sf::Event::Closed)
            window.close();
    }
#endif
}

void Game::update(sf::Time deltaTime)
{
    stateStack.update(deltaTime, context);
    if(connection)
    {
        connection->update(deltaTime);
    }
}

void Game::draw()
{
#ifdef GAME_USE_GLFW
    float r = (float) clearColor.r / 255.0f;
    float g = (float) clearColor.g / 255.0f;
    float b = (float) clearColor.b / 255.0f;
    float a = (float) clearColor.a / 255.0f;
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);

    stateStack.draw(context);

    glfwSwapBuffers(window);
#else
  #ifndef GAME_NO_RENDER_WINDOW
    window.clear(clearColor);
  #else
    float r = (float) clearColor.r / 255.0f;
    float g = (float) clearColor.g / 255.0f;
    float b = (float) clearColor.b / 255.0f;
    float a = (float) clearColor.a / 255.0f;
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
  #endif
    stateStack.draw(context);
    window.display();
#endif
}

#ifdef GAME_USE_GLFW
void GameCallbacks::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    sf::Event event;

    switch(action)
    {
    case GLFW_PRESS:
        event.type = sf::Event::KeyPressed;
        break;
    case GLFW_RELEASE:
        event.type = sf::Event::KeyReleased;
        break;
    default:
        return;
    }

    switch(key)
    {
    case GLFW_KEY_SPACE:
        event.key.code = sf::Keyboard::Space;
        break;
    case GLFW_KEY_APOSTROPHE:
        event.key.code = sf::Keyboard::Quote;
        break;
    case GLFW_KEY_COMMA:
        event.key.code = sf::Keyboard::Comma;
        break;
    case GLFW_KEY_MINUS:
        event.key.code = sf::Keyboard::Subtract;
        break;
    case GLFW_KEY_PERIOD:
        event.key.code = sf::Keyboard::Period;
        break;
    case GLFW_KEY_SLASH:
        event.key.code = sf::Keyboard::Slash;
        break;
    case GLFW_KEY_0:
        event.key.code = sf::Keyboard::Num0;
        break;
    case GLFW_KEY_1:
        event.key.code = sf::Keyboard::Num1;
        break;
    case GLFW_KEY_2:
        event.key.code = sf::Keyboard::Num2;
        break;
    case GLFW_KEY_3:
        event.key.code = sf::Keyboard::Num3;
        break;
    case GLFW_KEY_4:
        event.key.code = sf::Keyboard::Num4;
        break;
    case GLFW_KEY_5:
        event.key.code = sf::Keyboard::Num5;
        break;
    case GLFW_KEY_6:
        event.key.code = sf::Keyboard::Num6;
        break;
    case GLFW_KEY_7:
        event.key.code = sf::Keyboard::Num7;
        break;
    case GLFW_KEY_8:
        event.key.code = sf::Keyboard::Num8;
        break;
    case GLFW_KEY_9:
        event.key.code = sf::Keyboard::Num9;
        break;
    case GLFW_KEY_SEMICOLON:
        event.key.code = sf::Keyboard::SemiColon;
        break;
    case GLFW_KEY_EQUAL:
        event.key.code = sf::Keyboard::Equal;
        break;
    case GLFW_KEY_A:
        event.key.code = sf::Keyboard::A;
        break;
    case GLFW_KEY_B:
        event.key.code = sf::Keyboard::B;
        break;
    case GLFW_KEY_C:
        event.key.code = sf::Keyboard::C;
        break;
    case GLFW_KEY_D:
        event.key.code = sf::Keyboard::D;
        break;
    case GLFW_KEY_E:
        event.key.code = sf::Keyboard::E;
        break;
    case GLFW_KEY_F:
        event.key.code = sf::Keyboard::F;
        break;
    case GLFW_KEY_G:
        event.key.code = sf::Keyboard::G;
        break;
    case GLFW_KEY_H:
        event.key.code = sf::Keyboard::H;
        break;
    case GLFW_KEY_I:
        event.key.code = sf::Keyboard::I;
        break;
    case GLFW_KEY_J:
        event.key.code = sf::Keyboard::J;
        break;
    case GLFW_KEY_K:
        event.key.code = sf::Keyboard::K;
        break;
    case GLFW_KEY_L:
        event.key.code = sf::Keyboard::L;
        break;
    case GLFW_KEY_M:
        event.key.code = sf::Keyboard::M;
        break;
    case GLFW_KEY_N:
        event.key.code = sf::Keyboard::N;
        break;
    case GLFW_KEY_O:
        event.key.code = sf::Keyboard::O;
        break;
    case GLFW_KEY_P:
        event.key.code = sf::Keyboard::P;
        break;
    case GLFW_KEY_Q:
        event.key.code = sf::Keyboard::Q;
        break;
    case GLFW_KEY_R:
        event.key.code = sf::Keyboard::R;
        break;
    case GLFW_KEY_S:
        event.key.code = sf::Keyboard::S;
        break;
    case GLFW_KEY_T:
        event.key.code = sf::Keyboard::T;
        break;
    case GLFW_KEY_U:
        event.key.code = sf::Keyboard::U;
        break;
    case GLFW_KEY_V:
        event.key.code = sf::Keyboard::V;
        break;
    case GLFW_KEY_W:
        event.key.code = sf::Keyboard::W;
        break;
    case GLFW_KEY_X:
        event.key.code = sf::Keyboard::X;
        break;
    case GLFW_KEY_Y:
        event.key.code = sf::Keyboard::Y;
        break;
    case GLFW_KEY_Z:
        event.key.code = sf::Keyboard::Z;
        break;
    case GLFW_KEY_LEFT_BRACKET:
        event.key.code = sf::Keyboard::LBracket;
        break;
    case GLFW_KEY_BACKSLASH:
        event.key.code = sf::Keyboard::BackSlash;
        break;
    case GLFW_KEY_RIGHT_BRACKET:
        event.key.code = sf::Keyboard::RBracket;
        break;
    case GLFW_KEY_GRAVE_ACCENT:
        event.key.code = sf::Keyboard::Tilde;
        break;
    case GLFW_KEY_ESCAPE:
        event.key.code = sf::Keyboard::Escape;
        break;
    case GLFW_KEY_ENTER:
        event.key.code = sf::Keyboard::Return;
        break;
    case GLFW_KEY_TAB:
        event.key.code = sf::Keyboard::Tab;
        break;
    case GLFW_KEY_BACKSPACE:
        event.key.code = sf::Keyboard::BackSpace;
        break;
    case GLFW_KEY_INSERT:
        event.key.code = sf::Keyboard::Insert;
        break;
    case GLFW_KEY_DELETE:
        event.key.code = sf::Keyboard::Delete;
        break;
    case GLFW_KEY_RIGHT:
        event.key.code = sf::Keyboard::Right;
        break;
    case GLFW_KEY_LEFT:
        event.key.code = sf::Keyboard::Left;
        break;
    case GLFW_KEY_DOWN:
        event.key.code = sf::Keyboard::Down;
        break;
    case GLFW_KEY_UP:
        event.key.code = sf::Keyboard::Up;
        break;
    case GLFW_KEY_PAGE_UP:
        event.key.code = sf::Keyboard::PageUp;
        break;
    case GLFW_KEY_PAGE_DOWN:
        event.key.code = sf::Keyboard::PageDown;
        break;
    case GLFW_KEY_HOME:
        event.key.code = sf::Keyboard::Home;
        break;
    case GLFW_KEY_END:
        event.key.code = sf::Keyboard::End;
        break;
    case GLFW_KEY_PAUSE:
        event.key.code = sf::Keyboard::Pause;
        break;
    case GLFW_KEY_F1:
        event.key.code = sf::Keyboard::F1;
        break;
    case GLFW_KEY_F2:
        event.key.code = sf::Keyboard::F2;
        break;
    case GLFW_KEY_F3:
        event.key.code = sf::Keyboard::F3;
        break;
    case GLFW_KEY_F4:
        event.key.code = sf::Keyboard::F4;
        break;
    case GLFW_KEY_F5:
        event.key.code = sf::Keyboard::F5;
        break;
    case GLFW_KEY_F6:
        event.key.code = sf::Keyboard::F6;
        break;
    case GLFW_KEY_F7:
        event.key.code = sf::Keyboard::F7;
        break;
    case GLFW_KEY_F8:
        event.key.code = sf::Keyboard::F8;
        break;
    case GLFW_KEY_F9:
        event.key.code = sf::Keyboard::F9;
        break;
    case GLFW_KEY_F10:
        event.key.code = sf::Keyboard::F10;
        break;
    case GLFW_KEY_F11:
        event.key.code = sf::Keyboard::F11;
        break;
    case GLFW_KEY_F12:
        event.key.code = sf::Keyboard::F12;
        break;
    case GLFW_KEY_F13:
        event.key.code = sf::Keyboard::F13;
        break;
    case GLFW_KEY_F14:
        event.key.code = sf::Keyboard::F14;
        break;
    case GLFW_KEY_F15:
        event.key.code = sf::Keyboard::F15;
        break;
    case GLFW_KEY_KP_0:
        event.key.code = sf::Keyboard::Numpad0;
        break;
    case GLFW_KEY_KP_1:
        event.key.code = sf::Keyboard::Numpad1;
        break;
    case GLFW_KEY_KP_2:
        event.key.code = sf::Keyboard::Numpad2;
        break;
    case GLFW_KEY_KP_3:
        event.key.code = sf::Keyboard::Numpad3;
        break;
    case GLFW_KEY_KP_4:
        event.key.code = sf::Keyboard::Numpad4;
        break;
    case GLFW_KEY_KP_5:
        event.key.code = sf::Keyboard::Numpad5;
        break;
    case GLFW_KEY_KP_6:
        event.key.code = sf::Keyboard::Numpad6;
        break;
    case GLFW_KEY_KP_7:
        event.key.code = sf::Keyboard::Numpad7;
        break;
    case GLFW_KEY_KP_8:
        event.key.code = sf::Keyboard::Numpad8;
        break;
    case GLFW_KEY_KP_9:
        event.key.code = sf::Keyboard::Numpad9;
        break;
    case GLFW_KEY_KP_DECIMAL:
        event.key.code = sf::Keyboard::Period;
        break;
    case GLFW_KEY_KP_DIVIDE:
        event.key.code = sf::Keyboard::Slash;
        break;
    case GLFW_KEY_KP_MULTIPLY:
        event.key.code = sf::Keyboard::Multiply;
        break;
    case GLFW_KEY_KP_SUBTRACT:
        event.key.code = sf::Keyboard::Subtract;
        break;
    case GLFW_KEY_KP_ADD:
        event.key.code = sf::Keyboard::Add;
        break;
    case GLFW_KEY_KP_ENTER:
        event.key.code = sf::Keyboard::Return;
        break;
    case GLFW_KEY_KP_EQUAL:
        event.key.code = sf::Keyboard::Equal;
        break;
    case GLFW_KEY_LEFT_SHIFT:
        event.key.code = sf::Keyboard::LShift;
        break;
    case GLFW_KEY_LEFT_CONTROL:
        event.key.code = sf::Keyboard::LControl;
        break;
    case GLFW_KEY_LEFT_ALT:
        event.key.code = sf::Keyboard::LAlt;
        break;
    case GLFW_KEY_LEFT_SUPER:
        event.key.code = sf::Keyboard::LSystem;
        break;
    case GLFW_KEY_RIGHT_SHIFT:
        event.key.code = sf::Keyboard::RShift;
        break;
    case GLFW_KEY_RIGHT_CONTROL:
        event.key.code = sf::Keyboard::RControl;
        break;
    case GLFW_KEY_RIGHT_ALT:
        event.key.code = sf::Keyboard::RAlt;
        break;
    case GLFW_KEY_RIGHT_SUPER:
        event.key.code = sf::Keyboard::RSystem;
        break;
    case GLFW_KEY_MENU:
        event.key.code = sf::Keyboard::Menu;
        break;
    case GLFW_KEY_UNKNOWN:
    default:
        event.key.code = sf::Keyboard::Unknown;
        break;
    }

    event.key.alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS;
    event.key.control = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
    event.key.shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
    event.key.system = glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS;

    Game::staticEvents->push_back(event);
}

void GameCallbacks::charCallback(GLFWwindow* window, unsigned int codepoint)
{
    sf::Event event;

    event.type = sf::Event::TextEntered;
    event.text.unicode = codepoint;

    Game::staticEvents->push_back(event);
}

void GameCallbacks::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    sf::Event event;

    event.type = sf::Event::MouseMoved;
    event.mouseMove.x = std::round(xpos);
    event.mouseMove.y = std::round(ypos);

    Game::staticEvents->push_back(event);
}

void GameCallbacks::cursorEnterCallback(GLFWwindow* window, int entered)
{
    sf::Event event;

    event.type = entered ? sf::Event::MouseEntered : sf::Event::MouseLeft;

    Game::staticEvents->push_back(event);
}

void GameCallbacks::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    sf::Event event;

    event.type = action == GLFW_PRESS ? sf::Event::MouseButtonPressed : sf::Event::MouseButtonReleased;

    switch(button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        event.mouseButton.button = sf::Mouse::Left;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        event.mouseButton.button = sf::Mouse::Right;
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        event.mouseButton.button = sf::Mouse::Middle;
        break;
    case GLFW_MOUSE_BUTTON_4:
        event.mouseButton.button = sf::Mouse::XButton1;
        break;
    case GLFW_MOUSE_BUTTON_5:
        event.mouseButton.button = sf::Mouse::XButton2;
        break;
    default:
        return;
    }

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    event.mouseButton.x = std::round(xpos);
    event.mouseButton.y = std::round(ypos);

    Game::staticEvents->push_back(event);
}

void GameCallbacks::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if(xoffset != 0.0)
    {
        sf::Event event;

        event.type = sf::Event::MouseWheelScrolled;
        event.mouseWheelScroll.wheel = sf::Mouse::HorizontalWheel;
        event.mouseWheelScroll.delta = xoffset;

        event.mouseWheelScroll.x = std::round(xpos);
        event.mouseWheelScroll.y = std::round(ypos);

        Game::staticEvents->push_back(event);
    }
    if(yoffset != 0.0)
    {
        sf::Event event;

        event.type = sf::Event::MouseWheelScrolled;
        event.mouseWheelScroll.wheel = sf::Mouse::VerticalWheel;
        event.mouseWheelScroll.delta = yoffset;

        event.mouseWheelScroll.x = std::round(xpos);
        event.mouseWheelScroll.y = std::round(ypos);

        Game::staticEvents->push_back(event);
    }
}
#endif

