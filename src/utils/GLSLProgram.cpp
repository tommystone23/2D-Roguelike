#include "GLSLProgram.h"
#include "Errors.h"
#include <fstream>

GLSLProgram::GLSLProgram() : 
    _program_id(0),
    _vertex_shader_id(0),
    _fragment_shader_id(0)
{

}

GLSLProgram::~GLSLProgram()
{

}

void GLSLProgram::compile_shaders(const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path)
{
    _program_id = glCreateProgram();

    if((_vertex_shader_id = glCreateShader(GL_VERTEX_SHADER)) == 0)
    {
        fatal_error("Unable to create vertex shader");
    }
    if((_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER)) == 0)
    {
        fatal_error("Unable to create fragment shader");
    }

    /* Compile Vertex Shader */
    compile_shader(vertex_shader_file_path, _vertex_shader_id);

    /* Compile Fragment Shader */
    compile_shader(fragment_shader_file_path, _fragment_shader_id);
}

void GLSLProgram::link_shaders()
{
    // Attach shaders
    glAttachShader(_program_id, _vertex_shader_id);
    glAttachShader(_program_id, _fragment_shader_id);

    // Link the program
    glLinkProgram(_program_id);

    // Error checking/handling
    GLint is_linked = 0;
    glGetProgramiv(_program_id, GL_LINK_STATUS, (int*)&is_linked);
    if(is_linked == GL_FALSE)
    {
        GLint max_buffer = 0;
        glGetProgramiv(_program_id, GL_INFO_LOG_LENGTH, &max_buffer);
        char error[max_buffer];
        glGetProgramInfoLog(_program_id, max_buffer, &max_buffer, error);

        // Delete program and shaders
        glDeleteProgram(_program_id);
        glDeleteShader(_vertex_shader_id);
        glDeleteShader(_fragment_shader_id);

        fatal_error(std::string(error));
    }

    // Detach shaders after successful link
    glDetachShader(_program_id, _vertex_shader_id);
    glDetachShader(_program_id, _fragment_shader_id);
}

void GLSLProgram::compile_shader(const std::string file_path, GLuint id)
{
    // Read from vertex shader file
    std::ifstream shader_file(file_path);
    if(shader_file.fail())
    {
        fatal_error("Failed to open " + file_path);
    }

    std::string file_contents;
    std::string line;
    while(std::getline(shader_file, line))
    {
        file_contents += line + "\n";
    }
    shader_file.close();

    // Convert std::string to C const char* for glShaderSource
    const char* file_content_ptr = file_contents.c_str();
    glShaderSource(id, 1, &file_content_ptr, nullptr);
    glCompileShader(id);

    // Error checking
    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        GLint max_buffer = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_buffer);
        char error[max_buffer];
        glGetShaderInfoLog(id, max_buffer, &max_buffer, error);

        glDeleteShader(id);

        fatal_error(std::string(error));
    }
}

GLint GLSLProgram::get_uniform_location(const std::string &uniform_name)
{
    GLint location = glGetUniformLocation(_program_id, uniform_name.c_str());     
    if(location == GL_INVALID_INDEX)
    {
        fatal_error("Uniform " + uniform_name + " not found in shader");
    }

    return location;
}

void GLSLProgram::use()
{
    glUseProgram(_program_id);
}

void GLSLProgram::unuse()
{
    glUseProgram(0);
}