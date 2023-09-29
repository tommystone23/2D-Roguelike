#include "TextureCache.h"
#include "ImageLoader.h"

TextureCache::TextureCache()
{

}

TextureCache::~TextureCache()
{

}

GLTexture TextureCache::get_texture(std::string texture_file_path)
{
    // find() returns an iterator
    auto map_iterator = _texture_map.find(texture_file_path);

    // If texture does not exist in map, generate texture
    if(map_iterator == _texture_map.end())
    {
        // Create new texture
        GLTexture new_texture = ImageLoader::load_image(texture_file_path);
        // make_pair makes new pair for map
        _texture_map.insert(make_pair(texture_file_path, new_texture));

        return new_texture;
    }

    // Returns the value for the iterator if the texture is found
    return map_iterator->second;
}