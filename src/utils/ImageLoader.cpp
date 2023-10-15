#include "ImageLoader.h"
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "stb/stb_rect_pack.h"
#include "Errors.h"
# if SDL_BYTEORDER == SDL_BIG_ENDIAN
#  define RMASK 0xff000000
#  define GMASK 0x00ff0000
#  define BMASK 0x0000ff00
#  define AMASK 0x000000ff
# else
#  define RMASK 0x000000ff
#  define GMASK 0x0000ff00
#  define BMASK 0x00ff0000
#  define AMASK 0xff000000
# endif

GLTexture ImageLoader::load_image(std::string file_path)
{
    GLTexture texture = {};

    int number_of_channels;

    // Load texture
    unsigned char *texture_data = stbi_load(file_path.c_str(), &texture.width, &texture.height, &number_of_channels, STBI_rgb_alpha);
    if(texture_data)
    {
        // Generate and bind texture to the type (GL_TEXTURE_2D)
        glGenTextures(1, &texture.id);
        glBindTexture(GL_TEXTURE_2D, texture.id);
        // Upload texture data
        // Pass GL_RGBA for both parameters to include alpha values in texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
        // Set texture wrapping and filtering options
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE /*GL_REPEAT*/);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE /*GL_REPEAT*/);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // GL_NEAREST fixes texture edge flickering!!!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        printf("Failed to open texture data");
    }

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texture_data);

    return texture;
}

GLTexture ImageLoader::load_texture_atlas(const std::vector<std::string> &texture_paths,
                                            std::map<std::string, glm::vec4> *coords_out)
{
    std::vector<AtlasTexture> textures;
    textures.resize(texture_paths.size());

    int num_rects = texture_paths.size();
    stbrp_rect rects[num_rects];

    // TODO: Get coords for each texture. Hash based on file path.
    std::map<std::string, glm::vec4> coords;

    int i = 0;
    for(const std::string &path : texture_paths) {
        textures[i].data = stbi_load(path.c_str(), &textures[i].width, &textures[i].height, &textures[i].nb_channels, STBI_rgb_alpha);
        textures[i].path = path;
        if(textures[i].data) {
            textures[i].surface = SDL_CreateRGBSurfaceFrom((void*)textures[i].data, 
                                                            textures[i].width, 
                                                            textures[i].height, 
                                                            32, 
                                                            4 * textures[i].width, 
                                                            RMASK, 
                                                            GMASK, 
                                                            BMASK, 
                                                            AMASK);

            rects[i].id = i;
            rects[i].w = textures[i].width;
            rects[i].h = textures[i].height;
        }
        else
            printf("Failed to load texture");
        i++;
    }

    const int DEST_SURFACE_LENGTH = 1024;

    // Use this to pack rectangles before blitting
    stbrp_context context;
    // Arbitrarily large number for nodes
    int node_count = 4096 * 2;
    stbrp_node nodes[node_count];
    // Init context for pack rects function
    stbrp_init_target(&context, DEST_SURFACE_LENGTH, DEST_SURFACE_LENGTH, nodes, node_count);
    // Optimally pack rects into area
    stbrp_pack_rects(&context, rects, num_rects);

    //SDL_Surface *dest_surface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, DEST_SURFACE_LENGTH, DEST_SURFACE_LENGTH, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_Surface *dest_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, DEST_SURFACE_LENGTH, DEST_SURFACE_LENGTH, 32, RMASK, GMASK, BMASK, AMASK);

    for(size_t i = 0; i < num_rects; i++) {
        stbrp_rect rect = rects[i];
        AtlasTexture texture = textures[rect.id];

        // Get UV coords of texture
        glm::vec4 uv;
        uv.x = rect.x / DEST_SURFACE_LENGTH;
        uv.y = rect.y / DEST_SURFACE_LENGTH;
        uv.z = (rect.x + rect.w) / DEST_SURFACE_LENGTH;
        uv.w = (rect.y + rect.h) / DEST_SURFACE_LENGTH;
        coords.insert(std::pair<std::string, glm::vec4>(texture.path, uv));

        SDL_Rect dest_rect = { rect.x, rect.y, texture.width, texture.height };
        int ret = SDL_BlitSurface(texture.surface, NULL, dest_surface, &dest_rect);
        if(ret < 0) 
            printf("Failed to blit texture to surface");
    }

    // Convert to OpenGL texure and return GLTexture struct
    GLTexture texture = {};
    texture.width = DEST_SURFACE_LENGTH;
    texture.height = DEST_SURFACE_LENGTH;
    if(dest_surface->pixels)
    {
        // Generate and bind texture to the type (GL_TEXTURE_2D)
        glGenTextures(1, &texture.id);
        glBindTexture(GL_TEXTURE_2D, texture.id);
        // Upload texture data
        // Pass GL_RGBA for both parameters to include alpha values in texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dest_surface->pixels);
        // Set texture wrapping and filtering options
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE /*GL_REPEAT*/);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE /*GL_REPEAT*/);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // GL_NEAREST fixes texture edge flickering!!!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        printf("Failed to open texture data");
    }

    // Free memory
    SDL_FreeSurface(dest_surface);
    for(size_t i = 0; i < textures.size(); i++) {
        SDL_FreeSurface(textures[i].surface);
        stbi_image_free(textures[i].data);
    }

    if(coords_out)
        *coords_out = coords;

    return texture;
}

glm::vec4 ImageLoader::texture_coords_to_uv(const glm::vec4 &tex_coords, int width, 
                                            int height, bool inverted /* = false*/)
{
    glm::vec4 uv_coords;
    // Coords start at top left
    if(inverted) {
        uv_coords.x = tex_coords.x / (float)width;
        uv_coords.y = (height - tex_coords.y) / (float)height;
        uv_coords.z = tex_coords.z / (float)width;
        uv_coords.w = tex_coords.w / (float)height;
    }
    // Coords start at bottom left
    else {
        uv_coords.x = tex_coords.x / (float)width;
        uv_coords.y = tex_coords.y / (float)height;
        uv_coords.z = tex_coords.z / (float)width;
        uv_coords.w = tex_coords.w / (float)height;
    }

    return uv_coords;
}