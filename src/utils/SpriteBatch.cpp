/*
To use the sprite batch, you must call the functions in this order:
begin()
draw()
end()
render_batch()
*/
#include "SpriteBatch.h"
#include <algorithm>

Glyph::Glyph(const glm::vec4 &dest_rect, const glm::vec4 &uv_rect, 
            GLuint _texture, float _depth, const Color &color)
{
    texture = _texture;
    depth = _depth;

    top_left.set_color(color.r, color.g, color.b, color.a);
    top_left.set_position(dest_rect.x, dest_rect.y + dest_rect.w);
    top_left.set_uv(uv_rect.x, uv_rect.y + uv_rect.w);

    top_right.set_color(color.r, color.g, color.b, color.a);
    top_right.set_position(dest_rect.x + dest_rect.z, dest_rect.y + dest_rect.w);
    top_right.set_uv(uv_rect.x + uv_rect.z, uv_rect.y + uv_rect.w);

    bottom_left.set_color(color.r, color.g, color.b, color.a);
    bottom_left.set_position(dest_rect.x, dest_rect.y);
    bottom_left.set_uv(uv_rect.x, uv_rect.y);

    bottom_right.set_color(color.r, color.g, color.b, color.a);
    bottom_right.set_position(dest_rect.x + dest_rect.z, dest_rect.y);
    bottom_right.set_uv(uv_rect.x + uv_rect.z, uv_rect.y);
}

Glyph::Glyph(const glm::vec4 &dest_rect, const glm::vec4 &uv_rect, 
            GLuint _texture, float _depth, const Color &color, float angle)
{
    // rotation math
    glm::vec2 half_dimensions(dest_rect.z / 2.0f, dest_rect.w / 2.0f);

    // 4 corners of tile around the centre point
    glm::vec2 tl(-half_dimensions.x, half_dimensions.y);
    glm::vec2 tr(half_dimensions.x, half_dimensions.y);
    glm::vec2 bl(-half_dimensions.x, -half_dimensions.y);
    glm::vec2 br(half_dimensions.x, -half_dimensions.y);

    // Rotate points and add back half dimensions
    tl = rotate_point(tl, angle) + half_dimensions;
    tr = rotate_point(tr, angle) + half_dimensions;
    bl = rotate_point(bl, angle) + half_dimensions;
    br = rotate_point(br, angle) + half_dimensions;


    texture = _texture;
    depth = _depth;

    top_left.set_color(color.r, color.g, color.b, color.a);
    top_left.set_position(dest_rect.x + tl.x, dest_rect.y + tl.y);
    top_left.set_uv(uv_rect.x, uv_rect.y + uv_rect.w);

    top_right.set_color(color.r, color.g, color.b, color.a);
    top_right.set_position(dest_rect.x + tr.x, dest_rect.y + tr.y);
    top_right.set_uv(uv_rect.x + uv_rect.z, uv_rect.y + uv_rect.w);

    bottom_left.set_color(color.r, color.g, color.b, color.a);
    bottom_left.set_position(dest_rect.x + bl.x, dest_rect.y + bl.y);
    bottom_left.set_uv(uv_rect.x, uv_rect.y);

    bottom_right.set_color(color.r, color.g, color.b, color.a);
    bottom_right.set_position(dest_rect.x + br.x, dest_rect.y + br.y);
    bottom_right.set_uv(uv_rect.x + uv_rect.z, uv_rect.y);
}

glm::vec2 Glyph::rotate_point(glm::vec2 point, float angle)
{
    glm::vec2 new_vector;
    // Dot product rotation
    new_vector.x = point.x * cos(angle) - point.y * sin(angle);
    new_vector.y = point.x * sin(angle) + point.y * cos(angle);

    return new_vector;
}

SpriteBatch::SpriteBatch() : _vbo(0), _vao(0)
{
}

SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::init()
{
    create_vao();
}

/* Default Type: GlyphSortType::TEXTURE */
void SpriteBatch::begin(GlyphSortType sort_type)
{
    _sort_type = sort_type;
    _render_batches.clear();
    _glyphs.clear();
}

void SpriteBatch::end()
{
    _glyph_pointers.resize(_glyphs.size());
    for(size_t i = 0; i < _glyphs.size(); i++) {
        _glyph_pointers[i] = &_glyphs[i];
    }
    sort_glyphs();
    create_render_batches();
}

void SpriteBatch::draw(const glm::vec4 &dest_rect, const glm::vec4 &uv_rect, 
                        GLuint texture, float depth, const Color &color)
{
    _glyphs.emplace_back(dest_rect, uv_rect, texture, depth, color);
}

void SpriteBatch::draw(const glm::vec4 &dest_rect, const glm::vec4 &uv_rect, 
                        GLuint texture, float depth, const Color &color, float angle)
{
    _glyphs.emplace_back(dest_rect, uv_rect, texture, depth, color, angle);
}

void SpriteBatch::draw(const glm::vec4 &dest_rect, const glm::vec4 &uv_rect, 
                        GLuint texture, float depth, const Color &color, const glm::vec2 direction)
{
    // Looking forward/down
    const glm::vec2 default_direction(0.0f, -1.0f);
    float angle = acos(glm::dot(default_direction, direction));
    if(direction.x < 0.0f)
        angle = -angle;

    _glyphs.emplace_back(dest_rect, uv_rect, texture, depth, color, angle);
}

void SpriteBatch::render_batch()
{
    glBindVertexArray(_vao);

    for(size_t i = 0; i < _render_batches.size(); i++)
    {
        glBindTexture(GL_TEXTURE_2D, _render_batches[i].texture);
        glDrawArrays(GL_TRIANGLES, _render_batches[i].offset, _render_batches[i].num_vertices);
    }

    glBindVertexArray(0);
}

void SpriteBatch::create_render_batches()
{
    std::vector<Vertex> vertices;
    vertices.resize(_glyph_pointers.size() * 6);

    if(_glyph_pointers.empty())
        return;

    int offset = 0;
    int cur_vertex = 0;

    _render_batches.emplace_back(offset, 6, _glyph_pointers[0]->texture);
    vertices[cur_vertex++] = _glyph_pointers[0]->top_left;
    vertices[cur_vertex++] = _glyph_pointers[0]->bottom_left;
    vertices[cur_vertex++] = _glyph_pointers[0]->bottom_right;
    vertices[cur_vertex++] = _glyph_pointers[0]->bottom_right;
    vertices[cur_vertex++] = _glyph_pointers[0]->top_right;
    vertices[cur_vertex++] = _glyph_pointers[0]->top_left;
    offset += 6;

    for(size_t cur_glyph = 1; cur_glyph < _glyph_pointers.size(); cur_glyph++)
    {
        // Only add new glyph if texture is different than previous
        if(_glyph_pointers[cur_glyph]->texture != _glyph_pointers[cur_glyph-1]->texture)
        {
            _render_batches.emplace_back(offset, 6, _glyph_pointers[cur_glyph]->texture);
        }
        else
        {
            _render_batches.back().num_vertices += 6;
        }
        vertices[cur_vertex++] = _glyph_pointers[cur_glyph]->top_left;
        vertices[cur_vertex++] = _glyph_pointers[cur_glyph]->bottom_left;
        vertices[cur_vertex++] = _glyph_pointers[cur_glyph]->bottom_right;
        vertices[cur_vertex++] = _glyph_pointers[cur_glyph]->bottom_right;
        vertices[cur_vertex++] = _glyph_pointers[cur_glyph]->top_right;
        vertices[cur_vertex++] = _glyph_pointers[cur_glyph]->top_left;
        offset += 6;
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    // orphan the buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    // upload the data
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

    // unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteBatch::create_vao()
{
    // Generate vertex array object and vertex buffer object
    if(_vao == 0){
        glGenVertexArrays(1, &_vao);
    }
    glBindVertexArray(_vao);

    if(_vbo == 0){
        glGenBuffers(1, &_vbo);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    // Pointer to each vertex in buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // Pointer to color in vertex buffer. GL_TRUE because values need to be converted to floats
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    // Pointer to texture in vertex buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);

    // Unbind vao
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteBatch::sort_glyphs()
{
    switch(_sort_type)
    {
        case(GlyphSortType::NONE):
            break;
        case(GlyphSortType::BACK_TO_FRONT):
            std::stable_sort(_glyph_pointers.begin(), _glyph_pointers.end(), compare_back_to_front);
            break;
        case(GlyphSortType::FRONT_TO_BACK):
            std::stable_sort(_glyph_pointers.begin(), _glyph_pointers.end(), compare_front_to_back);
            break;
        case(GlyphSortType::TEXTURE):
            std::stable_sort(_glyph_pointers.begin(), _glyph_pointers.end(), compare_texture);
            break;
    }
}

bool SpriteBatch::compare_front_to_back(Glyph *a, Glyph *b)
{
    return (a->depth < (b->depth));
}

bool SpriteBatch::compare_back_to_front(Glyph *a, Glyph *b)
{
    return (a->depth > (b->depth));
}

bool SpriteBatch::compare_texture(Glyph *a, Glyph *b)
{
    return (a->texture < (b->texture));
}
