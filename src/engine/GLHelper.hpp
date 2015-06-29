
#ifndef GL_HELPER_HPP
#define GL_HELPER_HPP

#include <cstdlib>
#include <unordered_map>
#include <vector>
#include <memory>
#include <GL/glew.h>

namespace GLHelper
{
    /*
        An ElementManager keeps track of things like buffers stored with OpenGL.
        Multiple instances of this can therefore conflict with each other.
        Thus, ElementManager is a singleton.

        ElementManager assumes coordinates are 3D coordinates.
    */

    enum VerticesHint
    {
        AUTOMATIC,
        COLOR,
        COLOR_AND_TEX
    };

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
            bool vertexAdded;
            bool hasTextureCoords;
            bool finalized;
            // otherwise, is polygon
            bool isLine;
        };

        std::unordered_map<GLuint, std::unique_ptr<Element> > elementMap;
        GLuint vertexShader;
        GLuint vertexShaderWithTexture;
        GLuint fragmentShader;
        GLuint fragmentShaderWithTexture;
        GLuint shaderProgram;
        GLuint shaderProgramWithTexture;

    public:
        GLuint addVAO();
        void removeVAO(GLuint vao);

        /*
            3D coordinates + 4D color = 7 values per vertex
            3D coordinates + 4D color + 2D texture = 9 values per vertex
        */
        void finalize(GLuint vao, std::vector<float> vertices, std::vector<GLuint> elements, VerticesHint hint = AUTOMATIC, bool isDynamic = false);

        void draw();
    };

}

#endif

