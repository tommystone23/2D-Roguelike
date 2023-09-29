#ifndef IMAGELOADER_H
#define IMAGELOADER_H
#include "GLTexture.h"
#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

struct AtlasTexture 
{
    std::string path;
    unsigned char* data;
    SDL_Surface *surface;
    int width;
    int height;
    int nb_channels;
};

class ImageLoader
{
public:
    static GLTexture load_image(std::string file_path);
    static GLTexture load_texture_atlas(const std::vector<std::string> &texture_paths,
                                        std::map<std::string, glm::vec4> *coords_out);
    static glm::vec4 texture_coords_to_uv(const glm::vec4 &tex_coords, int width, 
                                        int height, bool inverted = false);
};

#endif