
#ifdef GAME_USE_GLFW

#ifndef GL_HELPER_HPP
#define GL_HELPER_HPP

#include <cstdlib>
#include <unordered_map>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace sf
{
    class Texture;
}

namespace GLHelper
{
    /*
        An ElementManager keeps track of things like buffers stored with OpenGL.
        Multiple instances of this can therefore conflict with each other.
        Thus, ElementManager is a singleton.

        ElementManager assumes coordinates are 3D coordinates.
    */

    class ElementManager
    {
    public:
        ~ElementManager();

        static ElementManager& getInstance();

    private:
        ElementManager();

        struct Element
        {
            typedef std::unique_ptr<Element> Ptr;

            Element();
            ~Element();

            GLuint vao;
            GLuint vbo;
            GLuint ebo;
            unsigned int drawIndices;
            const sf::Texture* texture;
            bool isReady;
        };

        std::unordered_map<GLuint, std::unique_ptr<Element> > elementMap;
        GLuint shaderProgram;
        GLuint shaderProgramWithTexture;

    public:
        GLuint addVAO();
        void removeVAO(GLuint vao);

        /*
            3D coordinates + 4D color = 7 values per vertex
            3D coordinates + 4D color + 2D texture = 9 values per vertex
        */
        void setupVAO(GLuint vao, std::vector<float> vertices, std::vector<GLuint> elements, const sf::Texture* texture = nullptr, bool isDynamic = false);

        void draw();
    };

}

#endif

#endif

