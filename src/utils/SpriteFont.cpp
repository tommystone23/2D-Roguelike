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
    if(TTF_Init() != 0)
    {
        fatal_error("ERROR::SDL_TTF: Could not init SDL_ttf Library");
    }

    TTF_Font *ttf_font = TTF_OpenFont(font, size);
    if (!ttf_font) 
    {
        fatal_error("ERROR::SDL_TTF: Could not open TTF file");
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    SDL_Color text_color = {255, 255, 255, 255};
    for (uint16_t c = 32; c < 128; c++)
    {
        SDL_Surface *text_surface = TTF_RenderGlyph_Blended(ttf_font, (char)c, text_color);
        if(!text_surface) 
        {
            fatal_error("ERROR::SDL_TTF: Could not create character glyph");
        }
        if(text_surface->format->format != SDL_PIXELFORMAT_RGBA32) {
            SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
            SDL_Surface *new_surface = SDL_ConvertSurface(text_surface, format, 0);
            SDL_FreeSurface(text_surface);
            SDL_FreeFormat(format);
            text_surface = new_surface;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            text_surface->w,
            text_surface->h,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            text_surface->pixels);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        int min_x;
        int max_y;
        TTF_GlyphMetrics(ttf_font, c, &min_x, NULL, NULL, &max_y, NULL);
        CharGlyph character = {
            texture,
            glm::vec2(text_surface->w, text_surface->h),
            glm::vec2(min_x, max_y)
        };

        _char_glyphs.insert(std::pair<char, CharGlyph>(c, character));
        SDL_FreeSurface(text_surface);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void SpriteFont::draw(SpriteBatch *batch, const char* s, glm::vec2 position, 
                        glm::vec2 scaling, float depth, Color tint) 
{
    glm::vec2 pos = position;
    for (int i = 0; s[i] != 0; i++) {
        char c = s[i];
        CharGlyph character = _char_glyphs[c];

        // Shift position based on baseline offset. 
        // This is important for letters such as p and q that appear above or below the baseline
        // float xpos = pos.x + character.baseline_offset.x * scaling.x;
        // float ypos = pos.y - (character.size.y - character.baseline_offset.y) * scaling.y;

        float xpos = pos.x;
        float ypos = pos.y;

        glm::vec2 new_pos(xpos, ypos);

        glm::vec4 destRect(new_pos, character.size * scaling);
        const glm::vec4 uv_rect(0.0f, 0.0f, 1.0f, 1.0f);

        batch->draw(destRect, uv_rect, character.texture, depth, tint);

        pos.x += character.size.x * scaling.x;
    }
}