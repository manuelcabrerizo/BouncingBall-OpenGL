#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>

#include "shader.h"

char* ReadFile(const char* file_path)
{
    FILE* file;
    char* buffer;
    file = fopen(file_path, "rb");
    if(file == NULL)
    {
        OutputDebugString("ERROR::LOADING::SHADER::FILE\n");
        return NULL;
    }

    // go to the end of the file 
    fseek(file, 0, SEEK_END);
    // get the size of the file, to alloc the memory we need
    long int file_size = ftell(file);
    // go back to the start of the file
    fseek(file, 0, SEEK_SET);
    // alloc the memory     
    buffer = (char*)malloc(file_size + 1);
    if(buffer == NULL)
    {
        OutputDebugString("ERROR::ALLOCATING::MEMORY::FOR::BUFFER\n");
        return NULL;
    }
    // store the content of the file in the buffer of chars
    fread((void*)buffer, file_size, 1, file);
    buffer[file_size] = 0; // null terminating string...
    fclose(file); // free FILE*...

    return buffer;

}

void LoadShader(
        Shader* shader,
        const char* vertex_path,
        const char* fragment_path)
{
    // buffers to store the content of our shaders...
    char* vertex_buffer = ReadFile(vertex_path);
    char* fragment_buffer = ReadFile(fragment_path);
    if(vertex_buffer == NULL)
    {
        OutputDebugString("ERROR::LOADING::VERTEX::SHADER\n");
        return;
    }
    if(fragment_buffer == NULL)
    {
        OutputDebugString("ERROR::LOADING::FRAGMENT::SHADER\n");
        return;
    }

    // Load OpengGL SHADERS
    unsigned int vertex_shader;
    unsigned int fragment_shader;
    int success;
    char info[512];

    // VERTEXT SHADER...
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_buffer, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info);
        OutputDebugString(info);
    }
    free(vertex_buffer); // free the memory becouse we already compile our shader

    // FRAGMENT SHADER...
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_buffer, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info);
        OutputDebugString(info);
    }
    free(fragment_buffer); // free the memory becouse we already compile our shader

    // Create the SHADER PROGRAM to be use in our engine...
    shader->id = glCreateProgram();
    glAttachShader(shader->id, vertex_shader);
    glAttachShader(shader->id, fragment_shader);
    glLinkProgram(shader->id);
    glGetProgramiv(shader->id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shader->id, 512, NULL, info);
        OutputDebugString(info);
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void UseShader(Shader* shader)
{
    glUseProgram(shader->id);
}




