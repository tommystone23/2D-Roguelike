#include "ResourceManager.h"

TextureCache ResourceManager::_texture_cache;

GLTexture ResourceManager::get_texture(std::string texture_file_path)
{
    return _texture_cache.get_texture(texture_file_path);
}