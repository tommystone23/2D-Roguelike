#ifndef SPRITE_FONT_H
#define SPRITE_FONT_H

#include <glm/glm.hpp>
#include <map>
#include "Vertex.h"
#include <SDL2/SDL_ttf.h>

class SpriteBatch;

struct CharGlyph {
    GLuint texture;
    glm::vec2 size;
    glm::vec2 baseline_offset;
};

class SpriteFont 
{
public:
    SpriteFont() {};
    SpriteFont(const char* font, int size);

    void init(const char* font, int size);

    void draw(SpriteBatch *batch, const char* s, glm::vec2 position, glm::vec2 scaling, 
            float depth, Color tint);

private:
    std::map<char, CharGlyph> _char_glyphs;
};
#endif // SPRITE_FONT_H