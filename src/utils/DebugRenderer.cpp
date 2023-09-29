#include "DebugRenderer.h"

DebugRenderer::DebugRenderer()
{
}

DebugRenderer::~DebugRenderer()
{
}

void DebugRenderer::init()
{
    // Setup buffers
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ibo);

    // Bind buffers to VAO
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

    // TODO: Vertex Attrib Pointers

    // Unbind
    glBindVertexArray(0);
}

void DebugRenderer::end()
{
}

void DebugRenderer::draw_box(const glm::vec4 &dest_rect, const Color &color, float angle)
{
}

void DebugRenderer::draw_circle(const glm::vec2 &centre_pos, const Color color, float radius)
{
}

void DebugRenderer::render()
{
    // TODO: Use glDrawElements to draw indices of frame
}

void DebugRenderer::dispose()
{
}
