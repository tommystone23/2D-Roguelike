#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H
#include "GLTexture.h"
#include <map>
#include <string>

class TextureCache
{
public:
    TextureCache();
    ~TextureCache();

    GLTexture get_texture(std::string texture_file_path);

private:
    std::map<std::string, GLTexture> _texture_map;

};

#endif