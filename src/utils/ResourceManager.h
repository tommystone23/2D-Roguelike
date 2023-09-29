#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include "TextureCache.h"
#include <string>

class ResourceManager
{
public:
    static GLTexture get_texture(std::string texture_file_path);

private:
    static TextureCache _texture_cache;

};

#endif