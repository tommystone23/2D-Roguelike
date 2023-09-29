#ifndef DEBUG_RENDERER_H
#define DEBUG_RENDERER_H

#include <glm/glm.hpp>
#include "Vertex.h"
#include <vector>
#include "GLSLProgram.h"



class DebugRenderer
{
public:
    DebugRenderer();
    ~DebugRenderer();

    void init();
    void end();
    void draw_box(const glm::vec4 &dest_rect, const Color &color, float angle);
    void draw_circle(const glm::vec2 &centre_pos, const Color color, float radius);
    void render();
    void dispose();

    struct DebugVertex {
        glm::vec2 position;
        Color color;
    };

private:
    GLSLProgram _program;
    std::vector<DebugVertex> _vertices;
    std::vector<GLuint> _indices;
    GLuint _vbo;
    GLuint _vao;
    GLuint _ibo; // Index Buffer Object

};

#endif // DEBUG_RENDERER_H