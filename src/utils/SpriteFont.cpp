#include "SpriteFont.h"

#include "SpriteBatch.h"
#include <iostream> 
#include "Errors.h"

#define MAX_TEXTURE_RES 4096

SpriteFont::SpriteFont(const char* font, int size) 
{
    init(font, size);
}

void SpriteFont::init(const char* font, int size) 
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        fatal_error("ERROR::FREETYPE: Could not init FreeType Library");
    }

    FT_Face face;
    if (FT_New_Face(ft, font, 0, &face))
    {
        fatal_error("ERROR::FREETYPE: Failed to load font");
    }

    FT_Set_Pixel_Sizes(face, 0, size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_ALPHA,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_ALPHA,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        CharGlyph character = {
            texture,
            glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top)};

        _char_glyphs.insert(std::pair<char, CharGlyph>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void SpriteFont::draw(SpriteBatch& batch, const char* s, glm::vec2 position, 
                        glm::vec2 scaling, float depth, Color tint) 
{
    glm::vec2 pos = position;
    for (int i = 0; s[i] != 0; i++) {
        char c = s[i];
        CharGlyph character = _char_glyphs[c];

        // Shift position based on baseline offset. 
        // This is important for letters such as p and q that appear above or below the baseline
        float xpos = pos.x + character.baseline_offset.x * scaling.x;
        float ypos = pos.y - (character.size.y - character.baseline_offset.y) * scaling.y;
        glm::vec2 new_pos(xpos, ypos);

        glm::vec4 destRect(new_pos, character.size * scaling);
        const glm::vec4 uv_rect(0.0f, 0.0f, 1.0f, 1.0f);

        batch.draw(destRect, uv_rect, character.texture, depth, tint);

        pos.x += character.size.x * scaling.x;
    }
}