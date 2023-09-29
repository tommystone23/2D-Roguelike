#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H
#include "GL/glew.h"
#include <string>

class GLSLProgram 
{
public:
    GLSLProgram();
    ~GLSLProgram();

    void compile_shaders(const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path);
    void link_shaders();
    GLint get_uniform_location(const std::string &uniform_name);
    void use();
    void unuse();
private:
    void compile_shader(const std::string file_path, GLuint id);

    GLuint _program_id;
    GLuint _vertex_shader_id;
    GLuint _fragment_shader_id;
};


#endif